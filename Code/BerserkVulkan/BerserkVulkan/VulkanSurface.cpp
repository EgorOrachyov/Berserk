/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkVulkan/VulkanSurface.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanPhysicalDevice.hpp>
#include <BerserkVulkan/VulkanCmdBufferManager.hpp>
#include <BerserkVulkan/VulkanUtils.hpp>
#include <limits>

namespace Berserk {
    namespace RHI {

        VulkanSurface::VulkanSurface(SharedPtr<Window> window, VkSurfaceKHR surface, class VulkanDevice &device)
                : mSurface(surface), mWindow(std::move(window)), mDevice(device) {
            assert(mSurface);
            assert(mWindow);

            // Subscribe to window framebuffer resize event to properly handle
            // swap chain recreation (since not all platforms properly handle it internally in KHR)
            mWindow->OnWindowEvent.Subscribe([this](const Window::EventData& event) {
                if (event.eventType == Window::EventType::FramebufferResized)
                    this->Resize(event.framebufferSize.x(), event.framebufferSize.y());
            });
        }

        VulkanSurface::~VulkanSurface() {
            // To ensure that no image is used in the rendering
            mDevice.WaitDeviceIdle();

            // Remember to disconnect from window updates
            mResizeEvent.Disconnect();

            ReleaseSwapChain();

            vkDestroySurfaceKHR(mDevice.GetInstance(), mSurface, nullptr);
            mSurface = nullptr;
        }

        void VulkanSurface::GetSupportInfo(VkPhysicalDevice device, VulkanSwapChainSupportInfo &supportInfo) const {
            BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &supportInfo.capabilities));

            uint32 formatCount = 0;
            BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount,nullptr));

            if (formatCount != 0) {
                supportInfo.formats.Resize(formatCount);
                BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, supportInfo.formats.GetData()));
            }

            uint32 presentModeCount = 0;
            BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr));

            if (presentModeCount != 0) {
                supportInfo.presentModes.Resize(presentModeCount);
                BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, supportInfo.presentModes.GetData()));
            }
        }

        void VulkanSurface::SelectProperties() {
            auto device = mDevice.GetPhysicalDevice();

            VulkanSwapChainSupportInfo supportInfo;
            GetSupportInfo(device->Get(), supportInfo);

            // Formats selection
            Array<VkFormat> formats;
            formats.Add(VK_FORMAT_R8G8B8A8_SRGB);
            formats.Add(VK_FORMAT_B8G8R8A8_SRGB);

            for (auto format: formats) {
                bool found = false;

                for (auto& available: supportInfo.formats) {
                    if (available.format == format) {
                        mFormat = available;
                        found = true;
                        break;
                    }
                }

                if (found)
                    break;
            }

            // Presentation modes
            bool immediate;
            for (auto mode: supportInfo.presentModes) {
                if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                    immediate = true;
                }
            }

            mModePerformance = immediate ? VK_PRESENT_MODE_IMMEDIATE_KHR : VK_PRESENT_MODE_FIFO_KHR;
            mModeVsync = VK_PRESENT_MODE_FIFO_KHR;

            // Depth stencil buffers formats
            TextureFormat potentialFormats[] = {
                TextureFormat::DEPTH24_STENCIL8,
                TextureFormat::DEPTH32F_STENCIL8,
            };

            TextureFormat selected = TextureFormat::Unknown;

            for (auto p: potentialFormats) {
                if (mDevice.GetSupportedFormats().Contains(p)) {
                    selected = p;
                    break;
                }
            }

            if (selected == TextureFormat::Unknown)
                BERSERK_EXCEPT(FatalError, "Failed to select surface depth-stencil image format");

            mDepthStencilFormat = VulkanDefs::GetTextureFormat(selected);
        }

        void VulkanSurface::CreateSwapChain() {
            auto& device = *mDevice.GetPhysicalDevice();
            auto& queues = *mDevice.GetQueues();
            auto& memManager = *mDevice.GetMemoryManager();
            auto& cmdBufferManager = *mDevice.GetCmdBufferManager();
            String surfaceName = mWindow->GetName().GetStr();

            BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.Get(), mSurface, &mCapabilities))

            static const uint32 MAX = std::numeric_limits<uint32>::max();

            if (mCapabilities.currentExtent.width != MAX ||
                mCapabilities.currentExtent.height != MAX) {
                mExtent = mCapabilities.currentExtent;
            } else {
                auto size = mWindow->GetSize();
                auto width = (uint32) size.x();
                auto height = (uint32) size.y();

                auto& maxExtent = mCapabilities.maxImageExtent;
                auto& minExtent = mCapabilities.minImageExtent;

                mExtent.width = Math::Utils::Clamp(width, minExtent.width, maxExtent.width);
                mExtent.height = Math::Utils::Clamp(height, minExtent.height, maxExtent.height);
            }

            assert(mCapabilities.minImageCount <= Limits::MAX_FRAMES_IN_FLIGHT);
            uint32 imageCount = 3;

            if (mCapabilities.maxImageCount > 0) {
                imageCount = Math::Utils::Clamp(imageCount, mCapabilities.minImageCount, mCapabilities.maxImageCount);
            } else {
                imageCount = Math::Utils::Max(imageCount, mCapabilities.minImageCount);
            }

            VkSwapchainKHR newSwapchain;
            auto& queueFamilyIndices = queues.GetUniqueFamilies();

            VkSwapchainCreateInfoKHR swapchainCreateInfo{};
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.surface = mSurface;
            swapchainCreateInfo.minImageCount = imageCount;
            swapchainCreateInfo.imageFormat = mFormat.format;
            swapchainCreateInfo.imageColorSpace = mFormat.colorSpace;
            swapchainCreateInfo.imageExtent = mExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            swapchainCreateInfo.imageSharingMode = queues.GetResourcesSharingMode();
            swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.GetData();
            swapchainCreateInfo.preTransform = mCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = mVsync ? mModeVsync : mModePerformance;
            swapchainCreateInfo.clipped = VK_FALSE;
            swapchainCreateInfo.oldSwapchain = mSwapchain;

            BERSERK_VK_CHECK(vkCreateSwapchainKHR(mDevice.GetDevice(), &swapchainCreateInfo, nullptr, &newSwapchain));

            // Release old swapchain is present
            ReleaseSwapChain();

            // Check that we don't have anything from previous swap chain
            assert(mSwapColorImages.IsEmpty());
            assert(mSwapColorImageViews.IsEmpty());
            assert(mSwapDepthStencilImages.IsEmpty());
            assert(mSwapDepthStencilImageViews.IsEmpty());
            assert(mSwapDepthStencilImageAllocations.IsEmpty());

            // Assign new one
            mSwapchain = newSwapchain;

            BERSERK_VK_CHECK(vkGetSwapchainImagesKHR(mDevice.GetDevice(), mSwapchain, &imageCount, nullptr))
            BERSERK_VK_NAME(mDevice.GetDevice(), mSwapchain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, "Swapchain " + surfaceName);

            mSwapColorImages.Resize(imageCount);
            BERSERK_VK_CHECK(vkGetSwapchainImagesKHR(mDevice.GetDevice(), mSwapchain, &imageCount, mSwapColorImages.GetData()))

            mSwapColorImageViews.Resize(imageCount);


            // Color attachments views
            for (size_t i = 0; i < imageCount; i++) {
                VkImageViewCreateInfo viewCreateInfo{};
                viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                viewCreateInfo.image = mSwapColorImages[i];
                viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                viewCreateInfo.format = mFormat.format;
                viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                viewCreateInfo.subresourceRange.baseMipLevel = 0;
                viewCreateInfo.subresourceRange.levelCount = 1;
                viewCreateInfo.subresourceRange.baseArrayLayer = 0;
                viewCreateInfo.subresourceRange.layerCount = 1;

                BERSERK_VK_CHECK(vkCreateImageView(mDevice.GetDevice(), &viewCreateInfo, nullptr, &mSwapColorImageViews[i]));

                BERSERK_VK_NAME(mDevice.GetDevice(), mSwapColorImages[i], VK_OBJECT_TYPE_IMAGE, surfaceName + "-" + String::From(i))
                BERSERK_VK_NAME(mDevice.GetDevice(), mSwapColorImageViews[i], VK_OBJECT_TYPE_IMAGE_VIEW, surfaceName + "-" + String::From(i))
            }

            // Depth buffers creation
            VkImageCreateInfo dsImageInfo{};
            dsImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            dsImageInfo.imageType = VK_IMAGE_TYPE_2D;
            dsImageInfo.extent.width = mExtent.width;
            dsImageInfo.extent.height = mExtent.height;
            dsImageInfo.extent.depth = 1;
            dsImageInfo.mipLevels = 1;
            dsImageInfo.arrayLayers = 1;
            dsImageInfo.format = mDepthStencilFormat;
            dsImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            dsImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            dsImageInfo.usage =
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            dsImageInfo.sharingMode = queues.GetResourcesSharingMode();
            dsImageInfo.queueFamilyIndexCount = queueFamilyIndices.GetSize();
            dsImageInfo.pQueueFamilyIndices = queueFamilyIndices.GetData();
            dsImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            dsImageInfo.flags = 0;

            mSwapDepthStencilImages.EnsureToAdd(imageCount);
            mSwapDepthStencilImageViews.EnsureToAdd(imageCount);
            mSwapDepthStencilImageAllocations.EnsureToAdd(imageCount);

            for (size_t i = 0; i < imageCount; i++) {
                auto allocation = memManager.AllocateImage(dsImageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

                VkImageViewCreateInfo dsViewInfo{};
                dsViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                dsViewInfo.image = allocation.image;
                dsViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                dsViewInfo.format = mDepthStencilFormat;
                dsViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                dsViewInfo.subresourceRange.baseMipLevel = 0;
                dsViewInfo.subresourceRange.levelCount = 1;
                dsViewInfo.subresourceRange.baseArrayLayer = 0;
                dsViewInfo.subresourceRange.layerCount = 1;

                VkImageView dsView;

                BERSERK_VK_CHECK(vkCreateImageView(mDevice.GetDevice(), &dsViewInfo, nullptr, &dsView));

                BERSERK_VK_NAME(mDevice.GetDevice(), allocation.image, VK_OBJECT_TYPE_IMAGE, surfaceName + "-" + String::From(i))
                BERSERK_VK_NAME(mDevice.GetDevice(), dsView, VK_OBJECT_TYPE_IMAGE_VIEW, surfaceName + "-" + String::From(i))

                mSwapDepthStencilImages.Add(allocation.image);
                mSwapDepthStencilImageViews.Add(dsView);
                mSwapDepthStencilImageAllocations.Add(allocation.allocation);
            }

            // Transition depth images layouts
            {
                auto cmd = cmdBufferManager.StartGraphicsCmd();
                auto queue = queues.FetchNextGraphicsQueue();
                auto& utils = *mDevice.GetUtils();

                for (auto image: mSwapDepthStencilImages) {
                    utils.BarrierImage2d(cmd,
                                         image, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
                                         0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                                         VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
                }

                cmdBufferManager.Submit(queue, cmd);
                queues.WaitIdle(queue);
            }

            // For proper resize handling
            mRequestedExtent = mExtent;
            // Set current active frame image layout
            mCurrentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        }

        void VulkanSurface::ReleaseSwapChain() {
            if (mSwapchain) {
                auto& memManager = *mDevice.GetMemoryManager();

                // Color attachments view clean up
                for (auto imageView: mSwapColorImageViews) {
                    vkDestroyImageView(mDevice.GetDevice(), imageView, nullptr);
                }

                // Depth stencil attachments views/images and allocations clean up
                for (size_t i = 0; i < mSwapDepthStencilImages.GetSize(); i++) {
                    VulkanMemoryManager::ImageAllocation allocation{};
                    allocation.image = mSwapDepthStencilImages[i];
                    allocation.allocation = mSwapDepthStencilImageAllocations[i];

                    memManager.DeallocateImage(allocation);
                    vkDestroyImageView(mDevice.GetDevice(), mSwapDepthStencilImageViews[i], nullptr);
                }

                vkDestroySwapchainKHR(mDevice.GetDevice(), mSwapchain, nullptr);

                mSwapchain = nullptr;
                mSwapColorImages.Clear();
                mSwapColorImageViews.Clear();
                mSwapDepthStencilImages.Clear();
                mSwapDepthStencilImageViews.Clear();
                mSwapDepthStencilImageAllocations.Clear();
            }
        }

        void VulkanSurface::Resize(uint32 newWidth, uint32 newHeight) {
            mRequestedExtent.width = newWidth;
            mRequestedExtent.height = newHeight;
        }

        void VulkanSurface::Recreate() {
            // To be sure, that nothing is used in the rendering.
            // In general, it is bad, but resize process is so expensive, so user will have to wait.
            mDevice.WaitDeviceIdle();

            // Automatically calls release if needed
            CreateSwapChain();

            // Advance version
            // So the fbo and render pass cache will be able to recreate
            // associated with this surface cached objects.
            mVersion += 1;
        }

        void VulkanSurface::AcquireNextImage(VkSemaphore semaphore) {
            assert(!mImageRequested);

            if (mRequestedExtent.width != mExtent.width ||
                mRequestedExtent.height != mExtent.height) {
                Recreate();
            }

            while (true) {
                auto timeout = std::numeric_limits<uint64>::max();
                auto vkResult = vkAcquireNextImageKHR(mDevice.GetDevice(), mSwapchain, timeout, semaphore,nullptr, &mImageIndex);

                if (vkResult == VK_SUCCESS) {
                    break;
                } else if (vkResult == VK_ERROR_OUT_OF_DATE_KHR || vkResult == VK_SUBOPTIMAL_KHR) {
                    Recreate();
                    break;
                } else {
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Failed to acquire next image to draw for {0}"), GetName());
                }
            }

            mImageRequested = true;
        }

        void VulkanSurface::NotifyPresented() {
            mImageRequested = false;
            mCurrentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }

        void VulkanSurface::NotifyUsedInRenderPass() {
            mCurrentLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        void VulkanSurface::TransitionLayoutBeforeDraw(VkCommandBuffer buffer) {
            // So, after presentation we want to transfer to color attachment optimal
            assert(mCurrentLayout != VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

            auto index = mImageIndex;
            auto image = mSwapColorImages[index];

            auto& utils = *mDevice.GetUtils();

            VkAccessFlags srcAccessMask = 0;
            VkAccessFlags dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            utils.BarrierImage2d(buffer,
                                 image, VK_IMAGE_ASPECT_COLOR_BIT,
                                 srcAccessMask, dstAccessMask,
                                 mCurrentLayout, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                 srcStageMask, dstStageMask);

            mCurrentLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        void VulkanSurface::TransitionLayoutBeforePresentation(VkCommandBuffer buffer) {
            // Before presentation we want to transfer to khr presentation
            auto index = mImageIndex;
            auto image = mSwapColorImages[index];

            auto& utils = *mDevice.GetUtils();

            VkAccessFlags srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            VkAccessFlags dstAccessMask = 0;
            VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

            utils.BarrierImage2d(buffer,
                                 image, VK_IMAGE_ASPECT_COLOR_BIT,
                                 srcAccessMask, dstAccessMask,
                                 mCurrentLayout, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                 srcStageMask, dstStageMask);

            mCurrentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }

        void VulkanSurface::OnReleased() const {
            Memory::Release(this);
        }

    }
}
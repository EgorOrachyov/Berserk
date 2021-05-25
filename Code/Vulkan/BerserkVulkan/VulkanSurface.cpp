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

namespace Berserk {
    namespace RHI {

        VulkanSurface::VulkanSurface(SharedPtr<Window> window, VkSurfaceKHR surface, class VulkanDevice &device)
                : mSurface(surface), mWindow(std::move(window)), mDevice(device) {
            assert(mSurface);
            assert(mWindow);
        }

        VulkanSurface::~VulkanSurface() {
            if (mSurface) {
                ReleaseSwapChain();

                vkDestroySurfaceKHR(mDevice.GetInstance(), mSurface, nullptr);
                mSurface = nullptr;
            }
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

            bool foundMailBox;

            for (auto mode: supportInfo.presentModes) {
                if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    foundMailBox = true;
                }
            }

            mModePerformance = foundMailBox ? VK_PRESENT_MODE_MAILBOX_KHR : VK_PRESENT_MODE_FIFO_KHR;
            mModeVsync = VK_PRESENT_MODE_FIFO_KHR;
        }

        void VulkanSurface::CreateSwapChain() {
            auto device = mDevice.GetPhysicalDevice();
            auto queues = mDevice.GetQueues();

            BERSERK_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->Get(), mSurface, &mCapabilities));

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

            assert(mCapabilities.minImageCount <= Limits::MAX_FRAMES_IN_FLY);
            uint32 imageCount = Math::Utils::Clamp(3u, 2u, Limits::MAX_FRAMES_IN_FLY);

            if (mCapabilities.maxImageCount > 0) {
                imageCount = Math::Utils::Clamp(imageCount, mCapabilities.minImageCount, mCapabilities.maxImageCount);
            } else {
                imageCount = Math::Utils::Max(imageCount, mCapabilities.minImageCount);
            }

            VkSwapchainKHR newSwapchain;

            uint32 queueFamilyIndices[3] = {};
            uint32 familiesCount;
            queues->GetUniqueFamilies(queueFamilyIndices, familiesCount);

            VkSwapchainCreateInfoKHR swapchainCreateInfo{};
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.surface = mSurface;
            swapchainCreateInfo.minImageCount = imageCount;
            swapchainCreateInfo.imageFormat = mFormat.format;
            swapchainCreateInfo.imageColorSpace = mFormat.colorSpace;
            swapchainCreateInfo.imageExtent = mExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            swapchainCreateInfo.imageSharingMode = queues->GetResourcesSharingMode();
            swapchainCreateInfo.queueFamilyIndexCount = familiesCount;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
            swapchainCreateInfo.preTransform = mCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = mVsync ? mModeVsync : mModePerformance;
            swapchainCreateInfo.clipped = VK_FALSE;
            swapchainCreateInfo.oldSwapchain = mSwapchain;

            BERSERK_VK_CHECK(vkCreateSwapchainKHR(mDevice.GetDevice(), &swapchainCreateInfo, nullptr, &newSwapchain));

            // Release old swapchain is present
            ReleaseSwapChain();

            // Assign new one
            mSwapchain = newSwapchain;

            BERSERK_VK_CHECK(vkGetSwapchainImagesKHR(mDevice.GetDevice(), mSwapchain, &imageCount, nullptr));

            mSwapColorImages.Resize(imageCount);
            BERSERK_VK_CHECK(vkGetSwapchainImagesKHR(mDevice.GetDevice(), mSwapchain, &imageCount, mSwapColorImages.GetData()));

            mSwapColorImageViews.Resize(imageCount);

            String surfaceName = mWindow->GetName().GetStr();

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
        }

        void VulkanSurface::ReleaseSwapChain() {
            if (mSwapchain) {
                for (auto imageView: mSwapColorImageViews) {
                    vkDestroyImageView(mDevice.GetDevice(), imageView, nullptr);
                }

                vkDestroySwapchainKHR(mDevice.GetDevice(), mSwapchain, nullptr);

                mSwapchain = nullptr;
                mSwapColorImages.Clear();
                mSwapColorImageViews.Clear();
            }
        }

        void VulkanSurface::OnReleased() const {
            Memory::Release(this);
        }

    }
}
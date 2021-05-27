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

#ifndef BERSERK_VULKANSURFACE_HPP
#define BERSERK_VULKANSURFACE_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>

namespace Berserk {
    namespace RHI {

        /** Stores complete info about swap chain and presentation surface */
        class VulkanSurface final: public RefCountedThreadSafe {
        public:
            VulkanSurface(SharedPtr<Window> window, VkSurfaceKHR surface, class VulkanDevice& device);
            ~VulkanSurface() override;

            void GetSupportInfo(VkPhysicalDevice device, VulkanSwapChainSupportInfo& supportInfo) const;
            void SelectProperties();
            void CreateSwapChain();
            void ReleaseSwapChain();

            void Resize(uint32 newWidth, uint32 newHeight);
            void Recreate();

            bool IsIndexRequested() const { return mImageRequested; }
            void AcquireNextImage(VkSemaphore semaphore);
            void TransitionLayoutAfterPresentation(VkCommandBuffer buffer);
            void TransitionLayoutBeforePresentation(VkCommandBuffer buffer);
            void NotifyPresented();
            uint32 GetImageIndexToDraw() const { return mImageIndex; }
            VkSemaphore GetImageAvailableSemaphore() const { return mImageSemaphore; }

            uint32 GetVersion() const { return mVersion; }
            uint32 GetWidth() const { return mExtent.width; }
            uint32 GetHeight() const { return mExtent.height; }
            uint32 GetFramesCount() const { return mSwapColorImages.GetSize(); }
            VkSurfaceKHR GetSurface() const { return mSurface; }
            VkSurfaceFormatKHR GetFormat() const { return mFormat; }
            VkSwapchainKHR GetSwapchain() const { return mSwapchain; }
            VkFormat GetDepthStencilFormat() const { return mDepthStencilFormat; }
            StringName GetName() const { return mWindow->GetName(); }

            const Array<VkImageView> &GetSwapColorImageViews() const { return mSwapColorImageViews; };
            const Array<VkImageView> &GetSwapDepthStencilImageViews() const { return mSwapDepthStencilImageViews; }

        protected:
            void OnReleased() const override;

        private:
            VkSurfaceKHR mSurface;
            VkSwapchainKHR mSwapchain = nullptr;

            VkExtent2D mRequestedExtent{};
            VkExtent2D mExtent{};
            VkFormat mDepthStencilFormat{};
            VkSurfaceFormatKHR mFormat{};
            VkSurfaceCapabilitiesKHR mCapabilities{};
            VkPresentModeKHR mModeVsync = VK_PRESENT_MODE_MAX_ENUM_KHR;
            VkPresentModeKHR mModePerformance = VK_PRESENT_MODE_MAX_ENUM_KHR;
            bool mVsync = false;
            uint32 mVersion = 0;           // Track the resize history of the surface

            uint32 mImageIndex{};          // Index of the swap chain image for rendering
            VkSemaphore mImageSemaphore{}; // Semaphore signaled when image available
            bool mImageRequested = false;  // True, if index of image is requested, and it was not used in presentation yet

            Array<VkImage> mSwapColorImages;
            Array<VkImageView> mSwapColorImageViews;
            Array<VkImage> mSwapDepthStencilImages;
            Array<VkImageView> mSwapDepthStencilImageViews;
            Array<VmaAllocation> mSwapDepthStencilImageAllocations;

            SharedPtr<Window> mWindow;
            EventHnd mResizeEvent;

            class VulkanDevice& mDevice;
        };


    }
}

#endif //BERSERK_VULKANSURFACE_HPP
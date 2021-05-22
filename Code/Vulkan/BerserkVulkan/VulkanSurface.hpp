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

namespace Berserk {
    namespace RHI {

        struct VulkanSwapChainSupportInfo {
            VkSurfaceCapabilitiesKHR capabilities{};
            Array<VkSurfaceFormatKHR> formats;
            Array<VkPresentModeKHR> presentModes;
        };

        /** Stores complete info about swap chain and presentation surface */
        class VulkanSurface {
        public:
            VulkanSurface(SharedPtr<Window> window, VkSurfaceKHR surface, class VulkanDevice& device);
            ~VulkanSurface();

            void GetSupportInfo(VkPhysicalDevice device, VulkanSwapChainSupportInfo& supportInfo) const;
            void SelectProperties();
            void CreateSwapChain();
            void ReleaseSwapChain();

            VkSurfaceKHR GetSurface() const { return mSurface; }

        private:
            VkSurfaceKHR mSurface;
            VkSwapchainKHR mSwapchain = nullptr;
            VkExtent2D mExtent{};
            VkSurfaceFormatKHR mFormat{};
            VkSurfaceCapabilitiesKHR mCapabilities;
            VkPresentModeKHR mModeVsync = VK_PRESENT_MODE_MAX_ENUM_KHR;
            VkPresentModeKHR mPerformance = VK_PRESENT_MODE_MAX_ENUM_KHR;
            bool mVsync = true;

            SharedPtr<Window> mWindow;
            class VulkanDevice& mDevice;
        };


    }
}

#endif //BERSERK_VULKANSURFACE_HPP
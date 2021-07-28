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

#ifndef BERSERK_VULKANSURFACEMANAGER_HPP
#define BERSERK_VULKANSURFACEMANAGER_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanSurface.hpp>

namespace Berserk {
    namespace RHI {

        /** Manages creation, update and look-up of OS user-defined rendering surface */
        class VulkanSurfaceManager {
        public:
            VulkanSurfaceManager(class VulkanDevice& device, const VulkanDeviceInitInfo& initInfo);
            VulkanSurfaceManager(const VulkanSurfaceManager&) = delete;
            VulkanSurfaceManager(VulkanSurfaceManager&&) = delete;
            ~VulkanSurfaceManager();

            /** @return Primary surface (used only during device initialization) */
            RefCounted<VulkanSurface> GetPrimarySurface();

            /** @return Attempts to find surface, if fails, creates new one */
            RefCounted<VulkanSurface> GetOrCreateSurface(const SharedPtr<Window> &window);

            /** Remove window surface */
            void RemoveSurface(const SharedPtr<Window> &window);

        private:
            // Map platform window to its surface in the vulkan
            HashTable<SharedPtr<Window>, RefCounted<VulkanSurface>> mSurfaces;
            // Provided by host window management API
            Function<VkResult(VkInstance,const SharedPtr<Window>&, VkSurfaceKHR&)> mClientSurfaceFactory;
            // Device and instance access
            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANSURFACEMANAGER_HPP
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

#include <BerserkVulkan/VulkanSurfaceManager.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>

namespace Berserk {
    namespace RHI {

        VulkanSurfaceManager::VulkanSurfaceManager(struct VulkanDevice &device, const VulkanDeviceInitInfo &initInfo)
                : mDevice(device), mClientSurfaceFactory(initInfo.clientSurfaceFactory) {

            VkSurfaceKHR surfaceKhr;
            VkInstance instance = mDevice.GetInstance();

            BERSERK_VK_CHECK(mClientSurfaceFactory(instance, initInfo.primaryWindow, surfaceKhr));

            auto surface = Memory::Make<VulkanSurface>(initInfo.primaryWindow, surfaceKhr, mDevice);
            auto surfaceRef = RefCounted<VulkanSurface>(surface);

            mSurfaces.Add(initInfo.primaryWindow, surfaceRef);
        }

        VulkanSurfaceManager::~VulkanSurfaceManager() {
            mSurfaces.Clear();
        }

        RefCounted<VulkanSurface> VulkanSurfaceManager::GetPrimarySurface() {
            assert(mSurfaces.GetSize() == 1);
            return (*mSurfaces.begin()).GetSecond();
        }

        void VulkanSurfaceManager::InitializePrimarySurface() {
            auto surfaceRef = GetPrimarySurface();

            surfaceRef->SelectProperties();
            surfaceRef->CreateSwapChain();
        }

        RefCounted<VulkanSurface> VulkanSurfaceManager::GetOrCreateSurface(const SharedPtr<Window> &window) {
            auto surfacePtr = mSurfaces.GetPtr(window);

            if (surfacePtr != nullptr)
                return *surfacePtr;

            VkSurfaceKHR surfaceKhr;
            VkInstance instance = mDevice.GetInstance();

            BERSERK_VK_CHECK(mClientSurfaceFactory(instance, window, surfaceKhr));

            auto surface = Memory::Make<VulkanSurface>(window, surfaceKhr, mDevice);
            auto surfaceRef = RefCounted<VulkanSurface>(surface);

            // Select surface properties (called once, even if surface recreated many times)
            surfaceRef->SelectProperties();

            // Initialize swap chain, after that surface ready for rendering
            surfaceRef->CreateSwapChain();

            // Add surface to the map
            mSurfaces.Add(window, surfaceRef);

            return surfaceRef;
        }
    }
}
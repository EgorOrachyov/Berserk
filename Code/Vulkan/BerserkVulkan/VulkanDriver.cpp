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

#include <BerserkVulkan/VulkanDriver.hpp>

namespace Berserk {
    namespace RHI {

        VulkanDriver::VkImpl::VkImpl() {
            // We need to create instance, device and initial surface

//            mDevice = Memory::Make<VulkanDevice>();
//            mDeferredResources = Memory::Make<VulkanDeferredResources>();
//            mContext = Memory::Make<VulkanContext>();
//            mCmdListManager = Memory::Make<AsyncCommandQueueConsumer<>>();
//
//            Provide(this);
        }

        VulkanDriver::VkImpl::~VkImpl() {
            if (IsInitialized()) {
                Memory::Release(mCmdListManager);
                Memory::Release(mContext);
                Memory::Release(mDeferredResources);
                Memory::Release(mDevice);

                Remove(this);
            }

        }

        bool VulkanDriver::VkImpl::IsInitialized() const {
            return mDevice != nullptr;
        }

        void VulkanDriver::VkImpl::FixedUpdate() {
            // Swap queues, pending ops for init or release
            mDeferredResources->BeginFrame();

            // Release resources. At this moment nowhere in the system references to these resources are presented
            mDeferredResources->ExecutePendingReleaseQueue();

            // Init all resources. They will be available for all subsequent cmd lists
            mDeferredResources->ExecutePendingInitQueue();

            // Execute pending queues and then swap (so next exec will be what currently is submitted)
            mCmdListManager->ExecutePending();

            // Finish deferred resources scope
            mDeferredResources->EndFrame();
        }

        Device &VulkanDriver::VkImpl::GetDevice() {
            return *mDevice;
        }

        Context &VulkanDriver::VkImpl::GetContext() {
            return *mContext;
        }

        VulkanDeferredResources & VulkanDriver::VkImpl::GetDeferredResourceContext() {
            return *mDeferredResources;
        }

        AsyncCommandQueue<> VulkanDriver::VkImpl::GetCommandQueue() {
            return mCmdListManager->CreateQueue();
        }

    }
}
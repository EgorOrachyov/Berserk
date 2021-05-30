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

#ifndef BERSERK_VULKANDRIVER_HPP
#define BERSERK_VULKANDRIVER_HPP

#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanDriver final: public Driver {
        public:
            class VkImpl final: public Driver::Impl {
            public:
                explicit VkImpl(VulkanDeviceInitInfo initStruct);
                ~VkImpl() override;

                bool IsInitialized() const;
                void FixedUpdate();

                Device &GetDevice() override;
                Context &GetContext() override;
                class VulkanDeferredResources &GetDeferredResourceContext();
                AsyncCommandQueue<> GetCommandQueue();

            private:
                class VulkanDevice* mDevice = nullptr;
                class VulkanContext* mContext = nullptr;
                class VulkanDeferredResources* mDeferredResources = nullptr;
                AsyncCommandQueueConsumer<> *mCmdListManager = nullptr;
            };

            static Device &GetDevice() {
                return VkImpl::Instance().GetDevice();
            }

            static Context &GetContext() {
                return VkImpl::Instance().GetContext();
            }

            static class VulkanContext &GetVulkanContext() {
                return (class VulkanContext&) VkImpl::Instance().GetContext();
            }

            static AsyncCommandQueue<> GetCommandQueue() {
                return ((VkImpl&) VkImpl::Instance()).GetCommandQueue();
            }

            static VulkanDeferredResources &GetDeferredResourceContext() {
                return ((VkImpl&) VkImpl::Instance()).GetDeferredResourceContext();
            }
        };

    }
}

#endif //BERSERK_VULKANDRIVER_HPP
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

#include <BerserkVulkan/VulkanCmdBufferPool.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanQueues.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        VulkanCmdBufferPool::VulkanCmdBufferPool(struct VulkanDevice &device, size_t allocStep)
                : mDevice(device), mQueues(*device.GetQueues()), mAllocStep(allocStep) {

            assert(allocStep > 0);

            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            poolInfo.pNext = nullptr;

            uint32 framesInFlight = Limits::MAX_FRAMES_IN_FLIGHT;

            mGraphics.Resize(framesInFlight);
            mTransfer.Resize(framesInFlight);

            for (int i = 0; i < framesInFlight; i++) {
                VkCommandPool& gp = mGraphics[i].pool;
                poolInfo.queueFamilyIndex = mQueues.GetGraphicsQueueFamilyIndex();

                BERSERK_VK_CHECK(vkCreateCommandPool(mDevice.GetDevice(), &poolInfo, nullptr, &gp));
                BERSERK_VK_NAME(mDevice.GetDevice(), gp, VK_OBJECT_TYPE_COMMAND_POOL, "Graphics pool " + String::From(i));

                VkCommandPool& tp = mTransfer[i].pool;
                poolInfo.queueFamilyIndex = mQueues.GetTransferQueueFamilyIndex();

                BERSERK_VK_CHECK(vkCreateCommandPool(mDevice.GetDevice(), &poolInfo, nullptr, &tp));
                BERSERK_VK_NAME(mDevice.GetDevice(), tp, VK_OBJECT_TYPE_COMMAND_POOL, "Transfer pool " + String::From(i));
            }
        }

        VulkanCmdBufferPool::~VulkanCmdBufferPool() {
            // To be sure, that nothing is used in rendering
            mDevice.WaitDeviceIdle();

            uint32 framesInFlight = Limits::MAX_FRAMES_IN_FLIGHT;

            for (int i = 0; i < framesInFlight; i++) {
                vkDestroyCommandPool(mDevice.GetDevice(), mGraphics[i].pool, nullptr);
                vkDestroyCommandPool(mDevice.GetDevice(), mTransfer[i].pool, nullptr);
            }
        }

        void VulkanCmdBufferPool::NextFrame(uint32 frameIndex) {
            // Note, that it up to the user to ensure synchronisation and resole overlapping

            mCurrentFrameIndex = frameIndex;
            mFetchIndex = mCurrentFrameIndex % Limits::MAX_FRAMES_IN_FLIGHT;

            BERSERK_VK_CHECK(vkResetCommandPool(mDevice.GetDevice(), mGraphics[mFetchIndex].pool, 0));
            BERSERK_VK_CHECK(vkResetCommandPool(mDevice.GetDevice(), mTransfer[mFetchIndex].pool, 0));

            mGraphics[mFetchIndex].nextToAllocate = 0;
            mTransfer[mFetchIndex].nextToAllocate = 0;
        }

        VkCommandBuffer VulkanCmdBufferPool::StartGraphicsCmd() {
            return StartBuffer(mGraphics[mFetchIndex]);
        }

        VkCommandBuffer VulkanCmdBufferPool::StartTransferCmd() {
            return StartBuffer(mTransfer[mFetchIndex]);
        }

        void VulkanCmdBufferPool::ExpandPool(VulkanCmdBufferPool::Pool &pool) {
            if (pool.nextToAllocate >= pool.cached.GetSize()) {
                // Next to allocate is out of cache size, so we need to expand
                size_t currentSize = pool.cached.GetSize();
                size_t newSize = currentSize + mAllocStep;
                size_t toAllocate = mAllocStep;

                // Allocate place for new buffers
                pool.cached.Resize(newSize);

                VkCommandBufferAllocateInfo allocateInfo{};
                allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocateInfo.commandPool = pool.pool;
                allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocateInfo.commandBufferCount = toAllocate;

                BERSERK_VK_CHECK(vkAllocateCommandBuffers(mDevice.GetDevice(), &allocateInfo, pool.cached.GetData() + currentSize));

                mTotalAllocated += toAllocate;
            }
        }

        VkCommandBuffer VulkanCmdBufferPool::StartBuffer(VulkanCmdBufferPool::Pool &pool) {
            ExpandPool(pool);

            VkCommandBuffer buffer = pool.cached[pool.nextToAllocate];
            pool.nextToAllocate += 1;

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            BERSERK_VK_CHECK(vkBeginCommandBuffer(buffer, &beginInfo));

            return buffer;
        }



    }
}
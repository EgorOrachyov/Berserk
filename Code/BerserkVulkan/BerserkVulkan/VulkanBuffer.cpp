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

#include <BerserkVulkan/VulkanBuffer.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanStagePool.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>

namespace Berserk {
    namespace RHI {

        VulkanBuffer::VulkanBuffer(struct VulkanDevice &device) : mDevice(device) {

        }

        VulkanBuffer::~VulkanBuffer() {
            if (mBuffer) {
                auto& man = *mDevice.GetMemoryManager();

                VulkanMemoryManager::Allocation allocation{};
                allocation.buffer = mBuffer;
                allocation.allocation = mAllocation;

                man.DeallocateBuffer(allocation);

                mBufferSize = 0;
                mBuffer = nullptr;
                mAllocation = nullptr;
            }
        }

        void VulkanBuffer::Initialize(VkDeviceSize size, VkBufferUsageFlags flags) {
            assert(size > 0);

            // Buffers are always available as transfer destinations
            VkBufferUsageFlags usageFlags = flags | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

            // Device only memory, we use staging for update
            VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

            auto& allocator = *mDevice.GetMemoryManager();
            auto allocation = allocator.AllocateBuffer(size, usageFlags, memoryPropertyFlags);

            mBufferSize = size;
            mBuffer = allocation.buffer;
            mAllocation = allocation.allocation;
        }

        void VulkanBuffer::Update(VkCommandBuffer cmdBuffer, VkDeviceSize byteOffset, VkDeviceSize byteSize, const void *memory) {
            assert(memory);
            assert(byteSize > 0);
            assert(byteSize + byteOffset <= mBufferSize);

            auto& stagingPool = *mDevice.GetStagePool();
            auto  vmaAlloc = mDevice.GetMemoryManager()->GetVmaAllocator();

            // Allocate staging buffer of required size, map it and update
            auto staging = stagingPool.AllocateBuffer(byteSize);

            void* mappedRegion;
            BERSERK_VK_CHECK(vmaMapMemory(vmaAlloc, staging.allocation, &mappedRegion));

            Memory::Copy(mappedRegion, memory, byteSize);
            vmaUnmapMemory(vmaAlloc, staging.allocation);

            // Copy staging buffer into our buffer (remember about offset)
            VkBufferCopy copyRegion{};
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = byteOffset;
            copyRegion.size = byteSize;

            vkCmdCopyBuffer(cmdBuffer, staging.buffer, mBuffer, 1, &copyRegion);
        }


        void VulkanBuffer::UpdateAsync(VkCommandBuffer cmdBuffer, VkDeviceSize byteOffset, VkDeviceSize byteSize, const void *memory) {

        }
    }
}
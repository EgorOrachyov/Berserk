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

#ifndef BERSERK_VULKANBUFFER_HPP
#define BERSERK_VULKANBUFFER_HPP

#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkVulkan/VulkanMemoryManager.hpp>

namespace Berserk {
    namespace RHI {

        /** Generic GPU-only buffer for implementation vertex, index and uniform buffers */
        class VulkanBuffer {
        public:
            explicit VulkanBuffer(class VulkanDevice& device);
            VulkanBuffer(const VulkanBuffer&) = delete;
            VulkanBuffer(VulkanBuffer&&) noexcept = delete;
            ~VulkanBuffer();

            void Initialize(VkDeviceSize size, VkBufferUsageFlags flags);
            void Update(VkCommandBuffer cmdBuffer, VkDeviceSize byteOffset, VkDeviceSize byteSize, const void* memory);
            void UpdateAsync(VkCommandBuffer cmdBuffer, VkDeviceSize byteOffset, VkDeviceSize byteSize, const void* memory);

        protected:
            VkDeviceSize mBufferSize = 0;
            VkBuffer mBuffer = nullptr;
            VmaAllocation mAllocation = nullptr;

            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANBUFFER_HPP
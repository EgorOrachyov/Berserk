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

#ifndef BERSERK_VULKANQUEUES_HPP
#define BERSERK_VULKANQUEUES_HPP

#include <BerserkVulkan/VulkanDefs.hpp>

namespace Berserk {
    namespace RHI {

        class VulkanQueues {
        public:
            explicit VulkanQueues(VkPhysicalDevice device, VkSurfaceKHR surface);

            void SetupQueuesFromDevice(VkDevice device);
            void GetQueuesCreateInfos(Array<VkDeviceQueueCreateInfo> &creatInfos,
                                      Array<float> &graphicsQueuesPriority,
                                      Array<float> &transferQueuesPriority,
                                      Array<float> &presentQueuesPriority) const;

            bool IsComplete() const;
            void GetUniqueFamilies(uint32 *queues, uint32& count) const;
            uint32 GetGraphicsQueueFamilyIndex() const { return mGraphicsQueueFamilyIndex; }
            uint32 GetTransferQueueFamilyIndex() const { return mTransferQueueFamilyIndex; }
            uint32 GetPresentQueueFamilyIndex() const { return mPresentQueueFamilyIndex; }

            VkQueue FetchNextGraphicsQueue();
            VkQueue FetchNextTransferQueue();
            VkQueue FetchNextPresentQueue();

        private:
            static const uint32 INVALID_QUEUE_INDEX = 0xffffffff;

            Array<VkQueueFamilyProperties> mQueueFamilyProperties;
            Array<VkQueue> mGraphicsQueues;
            Array<VkQueue> mTransferQueues;
            Array<VkQueue> mPresentQueues;

            uint32 mGraphicsQueueFamilyIndex = INVALID_QUEUE_INDEX;
            uint32 mTransferQueueFamilyIndex = INVALID_QUEUE_INDEX;
            uint32 mPresentQueueFamilyIndex = INVALID_QUEUE_INDEX;

            uint32 mCurrentGraphicsQueue = 0;
            uint32 mCurrentTransferQueue = 0;
            uint32 mCurrentPresentQueue = 0;

            float mDefaultPriority = 1.0f;
        };

    }
}

#endif //BERSERK_VULKANQUEUES_HPP
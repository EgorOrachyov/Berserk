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

#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        PFN_vkCreateDebugUtilsMessengerEXT VulkanDebug::vkCreateDebugUtilsMessengerEXT = nullptr;
        PFN_vkDestroyDebugUtilsMessengerEXT VulkanDebug::vkDestroyDebugUtilsMessengerEXT = nullptr;
        PFN_vkSetDebugUtilsObjectNameEXT VulkanDebug::vkSetDebugUtilsObjectNameEXT = nullptr;
        PFN_vkCmdBeginDebugUtilsLabelEXT VulkanDebug::vkCmdBeginDebugUtilsLabelEXT = nullptr;
        PFN_vkCmdEndDebugUtilsLabelEXT VulkanDebug::vkCmdEndDebugUtilsLabelEXT = nullptr;

        void VulkanDebug::LoadInstanceFunctions(VkInstance instance) {
            vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
            vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT) vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT");
            vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT) vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT");

            assert(vkCreateDebugUtilsMessengerEXT);
            assert(vkDestroyDebugUtilsMessengerEXT);
            assert(vkSetDebugUtilsObjectNameEXT);
            assert(vkCmdBeginDebugUtilsLabelEXT);
            assert(vkCmdEndDebugUtilsLabelEXT);
        }

        void VulkanDebug::AddDebugName(VkDevice device, void *object, VkObjectType objectType, const char *name) {
            if (vkSetDebugUtilsObjectNameEXT) {
                VkDebugUtilsObjectNameInfoEXT nameInfo{};
                nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
                nameInfo.pNext = nullptr;
                nameInfo.objectType = objectType;
                nameInfo.objectHandle = reinterpret_cast<uint64>(object);
                nameInfo.pObjectName = name;

                BERSERK_VK_CHECK(vkSetDebugUtilsObjectNameEXT(device, &nameInfo));
            }
        }

        void VulkanDebug::AddDebugName(VkDevice device, void *object, VkObjectType objectType, const String &name) {
            AddDebugName(device, object, objectType, name.GetStr_C());
        }

        void VulkanDebug::AddDebugName(VkDevice device, void *object, VkObjectType objectType, const StringName &name) {
            AddDebugName(device, object, objectType, name.GetStr_C());
        }

        void VulkanDebug::AddDebugName(VkDevice device, uint64 object, VkObjectType objectType, const char *name) {
            if (vkSetDebugUtilsObjectNameEXT) {
                VkDebugUtilsObjectNameInfoEXT nameInfo{};
                nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
                nameInfo.pNext = nullptr;
                nameInfo.objectType = objectType;
                nameInfo.objectHandle = object;
                nameInfo.pObjectName = name;

                BERSERK_VK_CHECK(vkSetDebugUtilsObjectNameEXT(device, &nameInfo));
            }
        }

        void VulkanDebug::AddDebugName(VkDevice device, uint64 object, VkObjectType objectType, const String &name) {
            AddDebugName(device, object, objectType, name.GetStr_C());
        }

        void VulkanDebug::AddDebugName(VkDevice device, uint64 object, VkObjectType objectType, const StringName &name) {
            AddDebugName(device, object, objectType, name.GetStr_C());
        }

        void VulkanDebug::BeginLabel(VkCommandBuffer buffer, const char *name, const Color &color) {
            if (vkCmdBeginDebugUtilsLabelEXT) {
                VkDebugUtilsLabelEXT labelExt{};
                labelExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
                labelExt.pNext = nullptr;
                labelExt.pLabelName = name;

                Memory::Copy(labelExt.color, color.GetValues(), sizeof(float) * 4);

                vkCmdBeginDebugUtilsLabelEXT(buffer, &labelExt);
            }
        }

        void VulkanDebug::BeginLabel(VkCommandBuffer buffer, const String &name, const Color &color) {
            BeginLabel(buffer, name.GetStr_C(), color);
        }

        void VulkanDebug::BeginLabel(VkCommandBuffer buffer, const StringName &name, const Color &color) {
            BeginLabel(buffer, name.GetStr_C(), color);
        }

        void VulkanDebug::EndLabel(VkCommandBuffer buffer) {
            if (vkCmdEndDebugUtilsLabelEXT) {
                vkCmdEndDebugUtilsLabelEXT(buffer);
            }
        }

    }
}
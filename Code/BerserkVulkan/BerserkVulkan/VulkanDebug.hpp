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

#ifndef BERSERK_VULKANDEBUG_HPP
#define BERSERK_VULKANDEBUG_HPP

#include <BerserkVulkan/VulkanDefs.hpp>

#ifdef BERSERK_DEBUG
    #define BERSERK_VK_NAME(device,object,type,name) \
        VulkanDebug::AddDebugName(device, object, type, name);
#else
    #define BERSERK_VK_NAME(device,object,type,name)
#endif

namespace Berserk {
    namespace RHI {

        class VulkanDebug {
        public:
            // Called once when the instance in initialized
            static void LoadInstanceFunctions(VkInstance instance);
            static void AddDebugName(VkDevice device, void* object, VkObjectType objectType, const char* name);
            static void AddDebugName(VkDevice device, void* object, VkObjectType objectType, const String& name);
            static void AddDebugName(VkDevice device, void* object, VkObjectType objectType, const StringName& name);

            static PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
            static PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
            static PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
        };

    }
}

#endif //BERSERK_VULKANDEBUG_HPP
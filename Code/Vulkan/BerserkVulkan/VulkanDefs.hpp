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

#ifndef BERSERK_VULKANDEFS_HPP
#define BERSERK_VULKANDEFS_HPP

#include <vulkan/vulkan.h>
#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Color.hpp>
#include <BerserkCore/Image/PixelData.hpp>

#define BERSERK_LOG_VK BERSERK_TEXT("VK")

#define BERSERK_VK_LOG_INFO(...)                                                            \
        BERSERK_LOG_INFO(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_WARNING(...)                                                         \
        BERSERK_LOG_WARNING(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_ERROR(...)                                                           \
        BERSERK_LOG_ERROR(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_FATAL(...)                                                           \
        BERSERK_LOG_FATAL(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_CHECK_ERROR(result)                                                         \
        if ((result) != VK_SUCCESS) { BERSERK_VK_LOG_ERROR("Function returned error"); }

#define BERSERK_VK_CHECK(function)                                                          \
        do { auto result = function; BERSERK_CHECK_ERROR(result) } while (false);           \

namespace Berserk {
    namespace RHI {

        class VulkanDefs {
        public:


        };



    }
}

#endif //BERSERK_VULKANDEFS_HPP
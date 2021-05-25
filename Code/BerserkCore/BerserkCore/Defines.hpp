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

#ifndef BERSERK_DEFINES_HPP
#define BERSERK_DEFINES_HPP

#include <cassert>

#define BERSERK_PLATFORM_WINDOWS  1
#define BERSERK_PLATFORM_LINUX    2
#define BERSERK_PLATFORM_MACOS    3

#define BERSERK_COMPILER_MSVC   1
#define BERSERK_COMPILER_GNUC   2
#define BERSERK_COMPILER_INTEL  3
#define BERSERK_COMPILER_CLANG  4

#define BERSERK_ARCHITECTURE_x86_32 1
#define BERSERK_ARCHITECTURE_x86_64 2

#define BERSERK_ENDIAN_LITTLE       1
#define BERSERK_ENDIAN_BIG          2
#define BERSERK_ENDIAN BERSERK_ENDIAN_LITTLE

#define BERSERK_PATH_TYPE_UNIX      1
#define BERSERK_PATH_TYPE_WINDOWS   2

// Platform setup
#if defined( __WIN32__ ) || defined( _WIN32 )
    #define BERSERK_PLATFORM BERSERK_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__)
    #define BERSERK_PLATFORM BERSERK_PLATFORM_MACOS
#else
    #define BERSERK_PLATFORM BERSERK_PLATFORM_LINUX
#endif

// Architecture setup
#if defined(__x86_64__) || defined(_M_X64)
    #define BERSERK_ARCH_TYPE BERSERK_ARCHITECTURE_x86_64
#else
    #error Unsupportet target architecture
#endif

// Path type
#if BERSERK_PLATFORM == BERSERK_PLATFORM_WINDOWS
    #define BERSERK_PATH_TYPE BERSERK_PATH_TYPE_WINDOWS
#else
    #define BERSERK_PATH_TYPE BERSERK_PATH_TYPE_UNIX
#endif

// Build type (defined in the cmake)
// #define BERSERK_DEBUG
// #define BERSERK_RELEASE
// #define BERSERK_EDITOR

// Macro to work with text and string literals
#ifndef BERSERK_TEXT
    // General macro for literals wrapping
    #define BERSERK_TEXT(text) u8 ## text
#endif

#ifndef BERSERK_TEXT_UTF8
    // Utf-8 macro for strings
    #define BERSERK_TEXT_UTF8(text) u8 text
#endif

#endif //BERSERK_DEFINES_HPP

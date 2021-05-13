/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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

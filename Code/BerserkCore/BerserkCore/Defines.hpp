/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_DEFINES_HPP
#define BERSERK_DEFINES_HPP

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


// Compiler setup
#if defined(__clang__)
    #define BERSERK_COMPILER BERSERK_COMPILER_CLANG
    #define BERSERK_COMP_VER __clang_version__
    #define BERSERK_THREADLOCAL __thread
    #define BERSERK_STDCALL __attribute__((stdcall))
    #define BERSERK_CDECL __attribute__((cdecl))
    #define BERSERK_FALLTHROUGH [[clang::fallthrough]];
#elif defined(__GNUC__)
    #define BERSERK_COMPILER BERSERK_COMPILER_GNUC
    #define BERSERK_COMP_VER (((__GNUC__)*100) + \
		(__GNUC_MINOR__*10) + \
		__GNUC_PATCHLEVEL__)
    #define BERSERK_THREADLOCAL __thread
    #define BERSERK_STDCALL __attribute__((stdcall))
    #define BERSERK_CDECL __attribute__((cdecl))
    #define BERSERK_FALLTHROUGH __attribute__((fallthrough));
#else
    #error Currently unsupported compiler setup
#endif


// Platform setup
#if defined( __WIN32__ ) || defined( _WIN32 )
    #define BERSERK_PLATFORM BERSERK_PLATFORM_WINDOWS
    #define BERSERK_TARGET_WINDOWS
#elif defined( __APPLE_CC__)
    #define BERSERK_PLATFORM BERSERK_PLATFORM_MACOS
    #define BERSERK_TARGET_MACOS
#else
    #define BERSERK_PLATFORM BERSERK_PLATFORM_LINUX
    #define BERSERK_TARGET_LINUX
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


// Build type
#ifdef _BERSERK_DEBUG
    #define BERSERK_DEBUG
#endif

#ifdef _BERSERK_RELEASE
    #define BERSERK_RELEASE
#endif

#ifdef _BERSERK_EDITOR
    #define BERSERK_EDITOR
#endif


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

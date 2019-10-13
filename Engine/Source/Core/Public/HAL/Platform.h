//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_PLATFORM_H
#define BERSERK_PLATFORM_H

#include <CMakeOptions/CMakeOptionsList.h>

////////////////////////////////////////////////

#ifndef PLATFORM_MAC
    #ifdef CPLATFORM_MAC
        #define PLATFORM_MAC 1
    #else
        #define PLATFORM_MAC 0
    #endif
#endif

#ifndef PLATFORM_WIN
    #ifdef CPLATFORM_WIN
        #define PLATFORM_WIN 1
    #else
        #define PLATFORM_WIN 0
    #endif
#endif

#ifndef PLATFORM_LINUX
    #ifdef CPLATFORM_LINUX
        #define PLATFORM_LINUX 1
    #else
        #define PLATFORM_LINUX 0
    #endif
#endif

////////////////////////////////////////////////

#ifndef TARGET_X86_64
    #define TARGET_X86_64 0
#endif

////////////////////////////////////////////////

#if PLATFORM_MAC
    #include <Platform/Mac/MacPlatform.h>
#endif

#if PLATFORM_WIN
    #include <Platform/Win/WinPlatform.h>
#endif

#if PLATFORM_LINUX
    #include <Platform/Linux/LinuxPlatform.h>
#endif

////////////////////////////////////////////////

#include <HAL/TypesCheck.h>
#include <HAL/Lock.h>
#include <HAL/Memory.h>

////////////////////////////////////////////////

#ifndef MODE_DEBUG
    #ifdef CMODE_DEBUG
        #define MODE_DEBUG 1
    #else
        #define MODE_DEBUG 0
    #endif
#endif

#ifndef MODE_RELEASE
    #ifdef CMODE_RELEASE
        #define MODE_RELEASE 1
    #else
        #define MODE_RELEASE 0
    #endif
#endif

#ifndef MODE_EDITOR
    #ifdef CMODE_EDITOR
        #define MODE_EDITOR 1
    #else
        #define MODE_EDITOR 0
    #endif
#endif

////////////////////////////////////////////////

#endif //BERSERK_PLATFORM_H

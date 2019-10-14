//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_PLATFORM_H
#define BERSERK_PLATFORM_H

#include <CMakeOptions/CMakeOptionsList.h>

////////////////////////////////////////////////

#ifndef PLATFORM_MAC
    #define PLATFORM_MAC 0
#endif

#ifndef PLATFORM_WIN
    #define PLATFORM_WIN 0
#endif

#ifndef PLATFORM_LINUX
    #define PLATFORM_LINUX 0
#endif

////////////////////////////////////////////////

#ifndef TARGET_X86_64
    #define TARGET_X86_64 0
#endif

////////////////////////////////////////////////

#if PLATFORM_MAC
    #include <Platform/Mac/MacPlatform.h>
#elif PLATFORM_WIN
    #include <Platform/Win/WinPlatform.h>
#elif PLATFORM_LINUX
    #include <Platform/Linux/LinuxPlatform.h>
#else
    #error "Platform is not specified"
#endif

////////////////////////////////////////////////

#include <HAL/TypesCheck.h>
#include <HAL/Lock.h>
#include <HAL/Memory.h>

////////////////////////////////////////////////

#ifndef MODE_DEBUG
    #define MODE_DEBUG 0
#endif

#ifndef MODE_RELEASE
    #define MODE_RELEASE 0
#endif

#ifndef MODE_EDITOR
    #define MODE_EDITOR 0
#endif

////////////////////////////////////////////////

#endif //BERSERK_PLATFORM_H

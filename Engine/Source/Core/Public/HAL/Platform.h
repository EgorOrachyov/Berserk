//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_PLATFORM_H
#define BERSERK_PLATFORM_H

////////////////////////////////////////////////

#ifndef PLATFORM_MAC
    #define PLATFORM_MAC 1
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
#endif

#if PLATFORM_WIN
    #include <Platform/Win/WinPlatform.h>
#endif

#if PLATFORM_LINUX
    #include <Platform/Linux/LinuxPlatform.h>
#endif

////////////////////////////////////////////////

#ifndef DEBUG
    #define DEBUG 0
#endif

#ifndef RELEASE
    #define RELEASE 0
#endif

#ifndef EDITOR
    #define EDITOR 0
#endif

////////////////////////////////////////////////

#endif //BERSERK_PLATFORM_H
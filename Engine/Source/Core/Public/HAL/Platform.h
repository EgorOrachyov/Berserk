/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

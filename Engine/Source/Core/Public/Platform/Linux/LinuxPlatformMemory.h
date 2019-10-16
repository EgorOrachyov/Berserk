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

#ifndef BERSERK_LINUXPLATFORMMEMORY_H
#define BERSERK_LINUXPLATFORMMEMORY_H

#include <Platform/Linux/LinuxPlatformTypes.h>

namespace Berserk {

    /** Linux platform memory operations provider */
    class LinuxPlatformMemory {
    public:
        static const uint64 DEFAULT_ALIGNMENT = 16;
        static void* malloc(uint64 size);
        static void* malloc(uint64 size, uint64 alignment);
        static void* realloc(void* original, uint64 size);
        static void free(void* pointer);
        static void copy(void* destination, const void* source, uint64 size);
        static void zero(void* source, uint64 size);
    };

    /** Export as Platform Memory class */
    typedef LinuxPlatformMemory PlatformMemory;

}

#endif //BERSERK_LINUXPLATFORMMEMORY_H

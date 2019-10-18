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

#include <Memory/Alloc.h>
#include <Misc/Error.h>

namespace Berserk {

    Alloc::~Alloc() {
        RAW_ERROR_CONDITION(mCallsAllocate == mCallsFree,
                "Memory leaks detected [allocate: %u][free: %u]", mCallsAllocate, mCallsFree);
    }

    void *Alloc::malloc(uint32 size) {
        void* memory = Memory::malloc(size);
        RAW_ERROR_CONDITION(memory != nullptr, "Cannot allocate memory [size: %u]", size);
        mCallsAllocate += 1;
        return memory;
    }

    void *Alloc::malloc(uint32 size, uint32 alignment) {
        void* memory = Memory::malloc(size, alignment);
        RAW_ERROR_CONDITION(memory != nullptr, "Cannot allocate aligned memory [size: %u][alignment: %u]", size, alignment);
        mCallsAllocate += 1;
        return memory;
    }

    void Alloc::free(void *pointer) {
        Memory::free(pointer);
        mCallsFree += 1;
    }

    Alloc& Alloc::getSingleton() {
        static Alloc alloc;
        return alloc;
    }

}
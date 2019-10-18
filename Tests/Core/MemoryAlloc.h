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

#ifndef BERSERKTESTS_MEMORYALLOC_H
#define BERSERKTESTS_MEMORYALLOC_H

#include <Memory/PoolAlloc.h>
#include "TestMacro.h"

using namespace Berserk;

struct MemoryAlloc {

    TEST_FUNCTION_OBJECT(PoolAlloc) {
        PoolAlloc memory(sizeof(int64));

        const int32 size = 32;
        int64* buffer[size];

        for (int64* &value: buffer) {
            value = (int64*) memory.malloc(sizeof(int64));
            printf("%p\n", value);
        }

        for(int64* value: buffer) {
            memory.free(value);
        }

        printf("Clean\n");
        memory.clean();

        for (int64* &value: buffer) {
            value = (int64*) memory.malloc(sizeof(int64));
            printf("%p\n", value);
        }

        for(int64* value: buffer) {
            memory.free(value);
        }
    }

    TEST_RUN() {
        TEST_SECTION_NAME("Memory managers");
        TEST_FUNCTION_CALL(PoolAlloc);
    }

};


#endif //BERSERKTESTS_MEMORYALLOC_H

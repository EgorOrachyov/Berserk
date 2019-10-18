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

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include <Memory/IAlloc.h>

namespace Berserk {

    /** General purpose allocator - Platform memory wrapper */
    class Alloc : public IAlloc {
    public:

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size) override;

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size, uint32 alignment) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /** @return Total allocate calls for this allocator */
        uint32 getCallsAllocate() const {
            return mCallsAllocate;
        }

        /** @return Total free calls for this allocator */
        uint32 getCallsFree() const {
            return mCallsFree;
        }

        /** @return Engine global allocator instance */
        static Alloc& getSingleton();

    private:

        Alloc() = default;
        ~Alloc() override;

        //todo: add atomic types
        int32 mCallsAllocate = 0;
        int32 mCallsFree = 0;

    };

}

#endif //BERSERK_ALLOCATOR_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
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

#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <cstring>

namespace Berserk {

    void Memory::Copy(void *destination, const void *source, size_t sizeInBytes) {
        if (sizeInBytes > 0)
            std::memcpy(destination, source, sizeInBytes);
    }

    void Memory::Set(void *destination, uint32 value, size_t sizeInBytes) {
        if (sizeInBytes > 0)
            std::memset(destination, (int) value, sizeInBytes);
    }

    void* Memory::Allocate(size_t sizeInBytes) {
        return System::Impl::Instance().Allocate(sizeInBytes);
    }

    void * Memory::Reallocate(void *memory, size_t sizeInBytes) {
        return System::Impl::Instance().Reallocate(memory, sizeInBytes);
    }

    void Memory::Deallocate(void *memory) {
        System::Impl::Instance().Deallocate(memory);
    }

    uint64 Memory::GetAllocateCalls() {
        return System::Impl::Instance().GetAllocateCallsCount();
    }

    uint64 Memory::GetDeallocateCalls() {
        return System::Impl::Instance().GetDeallocateCallsCount();
    }

}
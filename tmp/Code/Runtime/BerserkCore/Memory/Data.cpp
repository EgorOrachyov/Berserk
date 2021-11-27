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

#include <BerserkCore/Memory/Data.hpp>

namespace Berserk {

    Data::Data(uint64 size, void* ptr, ReleaseProc releaseProc, bool isMutable)
        : mRelease(std::move(releaseProc)),
        mPtr(ptr),
        mSize(size),
        mMutable(isMutable) {

    }

    Data::~Data()  {
        if (mPtr) {
            mRelease(mPtr);
            mPtr = nullptr;
            mSize = 0;
        }
    }

    RcPtr<Data> Data::Make(const void *data, uint64 sizeInBytes) {
        assert(data);
        assert(sizeInBytes > 0);

        auto ptr = Make(sizeInBytes);
        Memory::Copy(ptr->GetDataWrite(), data, sizeInBytes);
        ptr->MarkImmutable();
        return ptr;
    }

    RcPtr<Data> Data::Make(uint64 sizeInBytes) {
        assert(sizeInBytes > 0);

        auto buffer = Memory::Allocate(sizeInBytes);
        return RcPtr<Data>(Memory::Make<Data>(sizeInBytes, buffer, [](void*p){Memory::Deallocate(p);}, true));
    }

}
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

#ifndef BERSERK_DATA_HPP
#define BERSERK_DATA_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/RcPtr.hpp>

namespace Berserk {

    /**
     * @brief Ref counted memory buffer for safe data passing
     *
     * Data stores immutable memory buffer pointer, so the address and size of the region cannot be change.
     * The data itself inside buffer can be mutated, if data is created with mutable flag.
     */
    class Data final: public RefCountedThreadSafe {
    public:
        using ReleaseProc = Function<void(void*)>;

        ~Data() override;

        /** @return Pointer to the read-only memory with data  */
        const void* GetData() const { return mPtr; };

        /** @return Pointer to the memory with data  */
        void* GetDataWrite() const { return mMutable? mPtr: nullptr; };

        /** @return Size of the memory data */
        uint64 GetSize() const { return mSize; };

        /** @return True if this data buffer is mutable */
        bool IsMutable() const { return mMutable; }

        /** Lock data for mutation. Now it is immutable. */
        void MarkImmutable() { mMutable = false; }

        /**
         * Makes new immutable data from provided data buffer.
         * Uses system malloc for internal data storage allocation.
         *
         * @param data Pointer to data to copy into buffer
         * @param sizeInBytes Size in bytes of the buffer
         * @return Created data instance
         */
        static RcPtr<Data> Make(const void* data, uint64 sizeInBytes);

        /**
         * Makes new mutable data with specified size.
         * Use get data function to retrieve writable memory pointer.
         * Uses system malloc for internal data storage allocation.
         *
         * @param sizeInBytes Size in bytes of the data buffer
         * @return Created data instance
         */
        static RcPtr<Data> Make(uint64 sizeInBytes);

    public:
        /** [Internal Usage] Creates data instance */
        Data(uint64 size, void* ptr, ReleaseProc releaseProc, bool isMutable);

    private:
        ReleaseProc mRelease;
        void* mPtr = nullptr;
        uint64 mSize = 0;
        bool mMutable = false;
    };

}

#endif //BERSERK_DATA_HPP
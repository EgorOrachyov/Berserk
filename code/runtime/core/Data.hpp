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

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/templates/Ref.hpp>

#include <functional>

BRK_NS_BEGIN

/**
 * @class Data
 * @brief Generic shared byte data storage
 */
class Data final : public RefCnt {
public:
    using ReleaseProc = std::function<void(void *)>;

    BRK_API ~Data() override;

    /** @return Pointer to the read-only memory with data  */
    const void *GetData() const { return mPtr; };

    /** @return Pointer to the memory with data  */
    void *GetDataWrite() { return mMutable ? mPtr : nullptr; };

    /** @return Size of the memory data */
    size_t GetSize() const { return mSize; };

    /** @return True if this data buffer is mutable */
    bool IsMutable() const { return mMutable; }

    /** Lock data for mutation. Now it is immutable. */
    void MarkImmutable() { mMutable = false; }

    /**
     * Makes new data from provided data buffer.
     * Uses system malloc for internal data storage allocation.
     *
     * @param data Pointer to data to copy into buffer
     * @param sizeInBytes Size in bytes of the buffer
     *
     * @return Created data instance
     */
    BRK_API static Ref<Data> Make(const void *data, size_t sizeInBytes);

    /**
     * Makes new data with specified size.
     * Use get data function to retrieve writable memory pointer.
     * Uses system malloc for internal data storage allocation.
     *
     * @param sizeInBytes Size in bytes of the data buffer
     *
     * @return Created data instance
     */
    BRK_API static Ref<Data> Make(size_t sizeInBytes);

protected:
    /** [Internal Usage] Creates data instance */
    Data(size_t size, void *ptr, ReleaseProc releaseProc, bool isMutable);

private:
    ReleaseProc mRelease;
    void *mPtr = nullptr;
    size_t mSize = 0;
    bool mMutable = false;
};

BRK_NS_END

#endif//BERSERK_DATA_HPP

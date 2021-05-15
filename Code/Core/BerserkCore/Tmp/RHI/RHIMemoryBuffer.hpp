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

#ifndef BERSERK_RHIMEMORYBUFFER_HPP
#define BERSERK_RHIMEMORYBUFFER_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/TPtrShared.hpp>
#include <BerserkCore/Function.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {

    /**
     * @brief Memory Buffer
     *
     * Cpu memory buffer, used to transfer data from Cpu to the Gpu memory for
     * Gpu resources update, such as vertex, index, uniform buffers and textures.
     *
     * Buffer owns its memory region, which will be released on destruction.
     * User allowed to provide callback, which can be used for custom memory release.
     */
    class RHIMemoryBuffer {
    public:
        using UserCallback = Function<void(uint64 size, void* memory)>;

        RHIMemoryBuffer() = default;

        RHIMemoryBuffer(uint64 size, void* memory, TPtrShared<UserCallback> callback = nullptr) {
            if (memory && size > 0) {
                mSize = size;
                mMemory = memory;
                mCallback = std::move(callback);
            }
        }

        RHIMemoryBuffer(RHIMemoryBuffer&& other) noexcept {
            mSize = other.mSize;
            mMemory = other.mMemory;
            mCallback = std::move(other.mCallback);

            other.mSize = 0;
            other.mMemory = nullptr;
        }

        ~RHIMemoryBuffer() {
            if (mMemory) {
                if (HasUserCallback()) {
                    UserCallback& func = *mCallback;
                    func(mSize, mMemory);
                    mCallback = nullptr;
                }
                else {
                    if (Allocator* allocator = Application::Get().GetGlobalAllocator()) {
                        allocator->Free(mMemory);
                    }
                }

                mSize = 0;
                mMemory = nullptr;
            }
        }

        uint64 GetSize() const { return mSize; }
        void* GetMemory() const { return mMemory; }
        bool HasUserCallback() const { return mCallback != nullptr; }

        static RHIMemoryBuffer AllocateAndCopy(uint64 size, const void* memory) {
            if (memory && size > 0) {
                if (Allocator* allocator = Application::Get().GetGlobalAllocator()) {
                    void* allocated = allocator->Allocate(size);
                    Memory::Copy(allocated, memory, size);
                    return RHIMemoryBuffer(size, allocated);
                }
            }

            return RHIMemoryBuffer();
        }

    private:
        uint64 mSize = 0;
        void* mMemory = nullptr;
        TPtrShared<UserCallback> mCallback;
    };

}

#endif //BERSERK_RHIMEMORYBUFFER_HPP
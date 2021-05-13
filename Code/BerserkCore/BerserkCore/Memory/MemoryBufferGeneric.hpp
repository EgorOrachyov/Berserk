/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORYBUFFERGENERIC_HPP
#define BERSERK_MEMORYBUFFERGENERIC_HPP

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Memory/MemoryBuffer.hpp>

namespace Berserk {

    template<typename Alloc = Allocator>
    class MemoryBufferGeneric: public MemoryBuffer {
    public:
        ~MemoryBufferGeneric() override {
            if (mPtr) {
                mAlloc.Deallocate(mPtr);
                mPtr = nullptr;
                mSize = 0;
            }
        }

        void Resize(size_t newSize) {
            if (mPtr) {
                mAlloc.Deallocate(mPtr);
            }

            mSize = newSize;
            mPtr = mAlloc.Allocate(newSize);
        }

        void* GetData() {
            return mPtr;
        }

        const void *GetData() const override {
            return mPtr;
        }

        size_t GetSize() const override {
            return mSize;
        }

        bool IsDataReady() const override {
            return true;
        }

        static Ref<MemoryBufferGeneric<Alloc>> Create(Alloc alloc = Alloc()) {
            auto buffer = new (Memory::Allocate(sizeof(MemoryBufferGeneric<Alloc>))) MemoryBufferGeneric<Alloc>(std::move(alloc));
            return Ref<MemoryBufferGeneric<Alloc>>(buffer, false);
        }

        static Ref<MemoryBufferGeneric<Alloc>> Create(size_t byteSize, const void* data, Alloc alloc = Alloc()) {
            auto buffer = new (Memory::Allocate(sizeof(MemoryBufferGeneric<Alloc>))) MemoryBufferGeneric<Alloc>(std::move(alloc));

            buffer->Resize(byteSize);

            if (data) {
                Memory::Copy(buffer->GetData(), data, byteSize);
            }

            return Ref<MemoryBufferGeneric<Alloc>>(buffer, false);
        }

    protected:
        explicit MemoryBufferGeneric(Alloc alloc = Alloc())
                : mAlloc(std::move(alloc)) {

        }

        void OnReleased() const override {
            this->~MemoryBufferGeneric();
            Allocator().Deallocate((void*) this);
        }

    private:
        Alloc mAlloc;
        void* mPtr = nullptr;
        size_t mSize = 0;
    };

}

#endif //BERSERK_MEMORYBUFFERGENERIC_HPP
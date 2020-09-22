/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORYCOW_HPP
#define BERSERK_MEMORYCOW_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    /** Shared memory block resource, copied on write operations */
    class MemoryCow {
    public:
        MemoryCow() = default;
        MemoryCow(const void* data, uint64 size);
        MemoryCow(const MemoryCow& other);
        MemoryCow(MemoryCow&& other) noexcept;
        ~MemoryCow();

        MemoryCow& operator=(const MemoryCow& other);
        MemoryCow& operator=(MemoryCow&& other) noexcept;

        uint64 getBufferSize() const;
        uint64 getReferencesCount() const;

        void* getData();
        const void* getDataReadonly() const;

        operator bool() const { return isNotNull(); }
        bool isNull() const { return mMetaData == nullptr; }
        bool isNotNull() const { return mMetaData != nullptr; }

    private:

        class alignas(16) MetaData {
        public:
            MetaData(uint64 bufferSize);
            uint64 addRefs();
            uint64 release();
            uint64 getReferences();
            uint64 getBufferSize();
            void* getData() const;
        private:
            AtomicInt64 mReferences;
            uint64 mBufferSize;
        };

        static MetaData* createMeta(uint64 dataSize, const void* data);
        static MetaData* cloneMeta(MetaData* metaToClone);
        static void tryRelease(MetaData* meta);

        MetaData* mMetaData = nullptr;
    };

}

#endif //BERSERK_MEMORYCOW_HPP
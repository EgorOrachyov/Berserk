//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGMANAGER_H
#define BERSERK_STRINGMANAGER_H

#include <Misc/AssertDev.h>
#include <Threading/Mutex.h>
#include <Containers/TArray.h>
#include <Memory/PoolAllocator.h>

namespace Berserk
{

    class ENGINE_API StringManager
    {
    public:

        struct StringInfo
        {
        public:

            GENERATE_NEW_DELETE(StringInfo);

            explicit StringInfo(uint32 size) : mSize(size) { }

            void incReference() { mReferenceCount += 1; }

            void decReference() { mReferenceCount -= 1; }

            bool hasReferences() { return mReferenceCount > 0; }

            uint32 size() const { return mSize; }

            uint32 length() const { return mLength; }

            uint32 references() const { return mReferenceCount; }

            uint8* buffer() { return (uint8*)this + sizeof(StringInfo); }

            static uint32 nodeSize(uint32 bufferSize) { return sizeof(StringInfo) + bufferSize; }

        private:

            /** Length of the sting (without end symbol) */
            uint32 mLength = 0;

            /** Size of this string buffer */
            uint32 mSize = 0;

            /** umber of references to this string */
            uint32 mReferenceCount = 0;

        };

        /** Types of the string */
        enum EStringTypes
        {
            Size_32     = 0,
            Size_64     = 1,
            Size_128    = 2,
            Size_256    = 3,
            Size_512    = 4,

            Total       = 5
        };

        /** Memory pools for strings */
        typedef TArray<PoolAllocator> MemoryPool;

    public:

        explicit StringManager(uint32 charTypeSize);

        ~StringManager();

        /** Creates empty string */
        StringInfo& emptyNode();

        /** Creates string of suitable size */
        StringInfo& createNode(uint32 size);

        /** Deletes string (dec ref, if refs == 0, delete) */
        void deleteNode(StringInfo& node);

    public:

        /** Global char string manager */
        static StringManager& getCharStringManager();

        /** Global wchar string manager */
        static StringManager& getWCharStringManager();

    protected:

        /** @retun id of the best fit pool to allocate node */
        static EStringTypes bestFit(uint32 size);

        /** @return SIze of the string from its id type */
        static uint32 stringSize(EStringTypes type);

    private:

        uint32 mStringsUsage = 0;
        uint32 mTotalStringsCreated = 0;
        uint32 mTotalStringsDestroyed = 0;

        Mutex       mMutex;
        MemoryPool  mMemoryPool;
        StringInfo* mDefaultEmptyString = nullptr;


    };

} // namespace Berserk

#endif //BERSERK_STRINGMANAGER_H
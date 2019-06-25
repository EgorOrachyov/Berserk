//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_SHAREDPTRMANAGER_H
#define BERSERK_SHAREDPTRMANAGER_H

#include <Misc/Types.h>
#include <Threading/Mutex.h>
#include <Memory/PoolAllocator.h>

namespace Berserk
{

    class ENGINE_API SharedPtrManager
    {
    public:

        /** Contains info for unique shared resource */
        struct SharedPtrInfo
        {
        public:

            GENERATE_NEW_DELETE(SharedPtrInfo);

            SharedPtrInfo(IAllocator* allocator) : mAllocator(allocator)
            {

            }

            void incReference() { mReferenceCount += 1; }

            void decReference() { mReferenceCount -= 1; }

            bool hasReferences() const { return mReferenceCount > 0; }

            uint32 references() const { return mReferenceCount; }

            IAllocator* allocator() const { return mAllocator; }

        private:

            /** How much pointer point to this resource */
            uint32 mReferenceCount = 0;

            /** Allocator to delete this shared ptr */
            IAllocator* mAllocator = nullptr;

        };

        /** Function to destroy source */
        typedef void (*DeleteSource)(void* source, IAllocator* allocator);

    public:

        SharedPtrManager();

        ~SharedPtrManager();

        SharedPtrInfo* emptyNode();

        SharedPtrInfo* createNode(IAllocator* allocator);

        void incReference(SharedPtrInfo* node);

        void deleteNode(void* source, DeleteSource fun, SharedPtrInfo* node);

        uint32 getPtrUsage() const { return mPtrUsage; }

        uint32 getTotalPtrCreated() const { return mTotalPtrCreated; }

        uint32 getTotalPtrDestroyed() const { return mTotalPtrDestroyed; }

    public:

        /** Global manager instance */
        static SharedPtrManager& get();

    private:

        volatile uint32 mPtrUsage = 0;
        volatile uint32 mTotalPtrCreated = 0;
        volatile uint32 mTotalPtrDestroyed = 0;

        Mutex mMutex;
        PoolAllocator mMemoryPool;
        SharedPtrInfo* mDefaultEmpty;

    };

} // namespace Berserk

#endif //BERSERK_SHAREDPTRMANAGER_H
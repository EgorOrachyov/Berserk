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

    /**
     * Shared pointers manager is a memory pool for shared pointers' data.
     * Provides reference counting and destroying.
     *
     * @note Thread-Safe
     * @warning Should be used only by shared ptrs
     */
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

        /** Number of shared ptr nodes in single pool buffer */
        static const uint32 DEFAULT_EXPANDING_COUNT = 128;

    public:

        SharedPtrManager();

        ~SharedPtrManager();

        /** @return Empty shared ptr info */
        SharedPtrInfo* emptyNode();

        /** @return Allocated node for shared data */
        SharedPtrInfo* createNode(IAllocator* allocator);

        /** Adds reference to the ptr (called for copy operations) */
        void incReference(SharedPtrInfo* node);

        /**
         * Delete node whether it has 0 ref count
         * @param fun Function, used to destroy resource and free memory
         */
        void deleteNode(void* source, DeleteSource fun, SharedPtrInfo* node);

        /** @return Current number of used shared data (unique shared ptr) */
        uint32 getPtrUsage() const { return mPtrUsage; }

        /** @return Totally created unique shared pointers for the engine work time */
        uint32 getTotalPtrCreated() const { return mTotalPtrCreated; }

        /** @return Totally destroyed unique shared pointers for the engine work time */
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
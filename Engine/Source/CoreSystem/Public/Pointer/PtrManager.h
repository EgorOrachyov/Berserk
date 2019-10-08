//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_PTRMANAGER_H
#define BERSERK_PTRMANAGER_H

#include <HAL/Types.h>
#include <Threading/Mutex.h>
#include <Threading/AtomicTypes.h>
#include <Memory/PoolAllocator.h>

namespace Berserk
{

    /**
     * Shared and Weak pointers manager is a memory pool for pointers' data.
     * Provides reference counting and destroying.
     *
     * @note Thread-Safe
     * @warning Should be used only by shared and weak ptrs
     */
    class ENGINE_API PtrManager
    {
    public:

        /** Contains info for unique shared resource */
        struct PtrInfo
        {
        public:

            GENERATE_NEW_DELETE(PtrInfo);

            PtrInfo(IAllocator* allocator)
                : mAllocator(allocator),
                  mSharedCounter(0),
                  mWeakCounter(0)
            {

            }

            int32 incRefShared() { return mSharedCounter.fetch_add(1); }

            int32 decRefShared() { return mSharedCounter.fetch_sub(1); }

            int32 incRefWeak() { return mWeakCounter.fetch_add(1); }

            int32 decRefWeak() { return mWeakCounter.fetch_sub(1); }

            bool hasSharedRefs() const { return mSharedCounter.load() != 0; }

            bool hasWeakRefs() const { return mWeakCounter.load() != 0; }

            AtomicInt& getSharedCounter() { return mSharedCounter; }

            AtomicInt& getWeakCounter() { return mWeakCounter; }

            IAllocator* allocator() const { return mAllocator; }

        private:

            /** Number of referenced shared pointers */
            AtomicInt mSharedCounter;

            /** Number of referenced weak pointers */
            AtomicInt mWeakCounter;

            /** Allocator to delete this shared ptr */
            IAllocator* mAllocator = nullptr;

        };

        /** Function to destroy source */
        typedef void (*DeleteSource)(void* source, IAllocator* allocator);

        /** Number of shared ptr nodes in single pool buffer */
        static const uint32 DEFAULT_EXPANDING_COUNT = 128;

    public:

        PtrManager();

        ~PtrManager();

        /** @return Allocated node for shared data */
        PtrInfo* createNode(IAllocator* allocator);

        /**
         * Delete node whether it has 0 ref count
         * @note Called only by TShared ptr when shared ref count == 0
         * @param fun Function, used to destroy resource and free memory
         */
        void deleteNode_CallBySharedPtr(void *source, DeleteSource fun, PtrInfo *node);

        /** Final node delete when no weak ptr references */
        void deleteNode_CallByWeakPtr(PtrInfo* node);

        /** @return Current number of used shared data (unique shared ptr) */
        uint32 getPtrUsage() const { return mPtrUsage; }

        /** @return Totally created unique shared pointers for the engine work time */
        uint32 getTotalPtrCreated() const { return mTotalPtrCreated; }

        /** @return Totally destroyed unique shared pointers for the engine work time */
        uint32 getTotalPtrDestroyed() const { return mTotalPtrDestroyed; }

    public:

        /** Global manager instance */
        static PtrManager& get();

    private:

        volatile uint32 mPtrUsage = 0;
        volatile uint32 mTotalPtrCreated = 0;
        volatile uint32 mTotalPtrDestroyed = 0;

        Mutex mMutex;
        PoolAllocator mMemoryPool;

    };

} // namespace Berserk

#endif //BERSERK_PTRMANAGER_H
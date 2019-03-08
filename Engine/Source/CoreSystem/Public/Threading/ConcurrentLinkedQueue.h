//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_CONCURRENTLINKEDQUEUE_H
#define BERSERK_CONCURRENTLINKEDQUEUE_H

#include <mutex>
#include "Misc/Types.h"
#include "Misc/Buffers.h"
#include "Misc/Platform.h"
#include "Misc/UsageDescriptors.h"
#include "Logging/LogMacros.h"
#include "Threading/IRunnable.h"
#include "Containers/LinkedList.h"

namespace Berserk
{

    /**
     * Blocking linked list based queue for primary handling runnable tasks for
     * frame based thread pool. Uses mutex for synchronization in
     * the methods pop and push
     */
    template <typename T>
    class CORE_API ConcurrentLinkedQueue
    {
    public:

        /** Default initial size for queue */
        static const uint32 INITIAL_SIZE = 64;

    public:

        /** Creates queue and initializes its internal linked list*/
        explicit ConcurrentLinkedQueue(uint32 size = INITIAL_SIZE);

        ~ConcurrentLinkedQueue();

    public:

        /**
         * Pop first element in queue, delete it from queue of
         * null if queue is empty
         * @param[out] result   Pointer to write data about element (if it exist)
         * @param[out] notEmpty Pointer to bool to write [true] if has element of [false]
         */
        void pop(T* result, bool* notEmpty);

        /** add element in the end of the queue */
        void push(const T& element);

        /** @return Current count of elements in the queue */
        uint32 getSize();

    private:

        std::mutex mMutex;
        LinkedList<T> mList;

    };

    template <typename T>
    ConcurrentLinkedQueue<T>::ConcurrentLinkedQueue(uint32 size) : mList(size)
    {

    }

    template <typename T>
    ConcurrentLinkedQueue<T>::~ConcurrentLinkedQueue()
    {
        PUSH("Concurrent Linked Queue: delete %p", this);
    }

    template <typename T>
    void ConcurrentLinkedQueue<T>::push(const T &element)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mList += element;
    }

    template <typename T>
    void ConcurrentLinkedQueue<T>::pop(T *result, bool *notEmpty)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mList.getSize() > 0)
        {
            memcpy(result, mList.getFirst(), sizeof(T));
            mList.removeFirst();
            *notEmpty = true;
        }
        else *notEmpty = false;
    }

    template <typename T>
    uint32 ConcurrentLinkedQueue<T>::getSize()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mList.getSize();
    }


} // namespace Berserk

#endif //BERSERK_CONCURRENTLINKEDQUEUE_H
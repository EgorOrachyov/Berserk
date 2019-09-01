//
// Created by Egor Orachyov on 2019-08-28.
//

#ifndef BERSERK_COMMANDQUEUE_H
#define BERSERK_COMMANDQUEUE_H

#include <Misc/Allocatable.h>
#include <Containers/TArray.h>
#include <Threading/AtomicTypes.h>
#include <Threading/Async.h>
#include <Resource/PtrUtils.h>

namespace Berserk
{

    /** Execute class function to run particular task on queue */
    class CORE_EXPORT Function
    {
    public:

        virtual ~Function() = default;

        /** Called by queue accessor */
        virtual void operator()() = 0;

    };


    /** Std based command function */
    typedef std::function<void()> StdFunction;

    /** Engine function object ptr */
    typedef TSharedPtr<Function> EngineFunction;


    /** Single command for the queue */
    class CORE_EXPORT QueueCommand : public Allocatable
    {
    public:

        /** Command from std function */
        explicit QueueCommand(StdFunction& function, Async async = Async())
                : mAsync(std::move(async)),
                  mStdFunction(std::move(function))
        { }

        /** Command from engine function */
        explicit QueueCommand(EngineFunction& function,  Async async = Async())
                : mAsync(std::move(async)),
                  mEngineFunction(std::move(function))
        { }

        /** Run this command function */
        void execute()
        {
            if (mEngineFunction.isPresent())
            {
                Function& fun = mEngineFunction.get();
                fun();
            }
            else
            {
                mStdFunction();
            }

            mAsync._complete();
        }

        /** @return Async handler [could be empty] */
        const Async& getAsync() const { return mAsync; }

        /** @return Std based function [could be empty] */
        const StdFunction& getStdFunction() const { return mStdFunction; }

        /** @return Engine based execute function [could be empty] */
        const EngineFunction& getEngineFunction() const { return mEngineFunction; }

    private:

        Async mAsync;
        StdFunction mStdFunction;
        EngineFunction mEngineFunction;

    };


    /** Single queue stores commands in the buffer */
    typedef TArray<QueueCommand> CommandQueueBuffer;


    /**
     * Allows to submit commands from multiple threads to be executed on the one thread.
     * After all commands submitted you must call flush() method.
     *
     * @note Primary must be used as communication interface between main thread
     *       and engine subsystem, executed somewhere in pool thread.
     */
    class CORE_EXPORT CommandQueue
    {
    public:

        /**
         * Construct ready-to-use queue
         *
         * @param defaultSize Num of command to preallocate inside the queue
         *
         * @param miscAlloc Allocator used to allocate async handles data and
         *                  engine function handlers. Supposed to be used double
         *                  frame allocator for allocating such things.
         *
         * @param queueAlloc Allocator used to allocate queues and its internal
         *                   buffers. Because of queues are reused, this allocator must
         *                   provide long-living allocations. System allocator suites well.
         *
         * @note If you use double frame allocator to allocate misc purposes, you
         *       must be sure, that all the commands executed in time of current and the next frame.
         */
        explicit CommandQueue(uint32 defaultSize = DEFAULT_QUEUE_SIZE,
                IAllocator& miscAlloc = Allocator::get(),
                IAllocator& queueAlloc = Allocator::get());

        /** Queue function to execute */
        void queue(StdFunction function);

        /** Queue function to execute */
        void queue(EngineFunction function);

        /** Construct engine function to execute via internal memory allocator */
        template <typename T, typename ... TArgs>
        void queue(TArgs&& ... args)
        {
            auto function = mem_new_shared_alloc<T>(mMiscAlloc, std::forward<TArgs>(args) ...);
            queue(function);
        }

        /**
         * Queue function to execute
         * @return Async handler to know, when the command is executed
         */
        Async queueReturn(StdFunction function);

        /**
         * Queue function to execute
         * @return Async handler to know, when the command is executed
         */
        Async queueReturn(EngineFunction function);

        /**
         * Construct engine function to execute via internal memory allocator
         * @return Async handler to know, when the command is executed
         */
        template <typename T, typename ... TArgs>
        Async queueReturn(TArgs&& ... args)
        {
            auto function = mem_new_shared_alloc<T>(mMiscAlloc, std::forward<TArgs>(args) ...);
            return queueReturn(function);
        }

        /**
         * Flushes currently submitted commands to be executed by receiver
         * (normally this function should be called once per frame)
         */
        void flush();

        /**
         * @note For receiver thread
         * @return Queue for execution (possibly empty)
         */
        TSharedPtr<CommandQueueBuffer> getSubmitQueue();

        /**
         * Add empty executed queue for reuse
         * @note For receiver thread
         * @param queue To add [must be empty]
         */
        void addEmptyQueue(TSharedPtr<CommandQueueBuffer> queue);

    public:

        /** Num of commands to preallocate queue buffer */
        static const uint32 DEFAULT_QUEUE_SIZE = 128;

    private:

        /** Allocates all queues and its buffers */
        IAllocator& mQueueAlloc;

        /** Allows to allocate engine functions and async handlers data */
        IAllocator& mMiscAlloc;

        /** Preallocate queue buffer size */
        uint32 mQueueDefaultSize;

        /** Controls access to submitted and empty queues */
        Mutex mMutex;

        /** Current queue used to submit commands by submit thread */
        TSharedPtr<CommandQueueBuffer> mCurrentQueue;

        /** Queues ready to be process by receiver thread  */
        TArray<TSharedPtr<CommandQueueBuffer>> mSubmitedQueues;

        /** Empty queues for reuse */
        TArray<TSharedPtr<CommandQueueBuffer>> mEmptyQueues;

    };

} // namespace Berserk




#endif //BERSERK_COMMANDQUEUE_H

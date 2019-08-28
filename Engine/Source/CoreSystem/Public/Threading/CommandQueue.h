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
     * Allows to submit commands from single thread to be executed on the other thread.
     * After all commands submitted you must call flush() method.
     *
     * @note Primary must be used as communication interface between main thread
     *       and engine subsystem, executed in the somewhere in pool thread.
     *
     * @note Only one thread to one thread communication.
     *       If you want to submit commands from multiple threads you must
     *       use multi command queue [?]
     */
    class CORE_EXPORT CommandQueue
    {
    public:

        explicit CommandQueue(uint32 defaultSize = DEFAULT_QUEUE_SIZE,
                IAllocator& miscAlloc = Allocator::get(),
                IAllocator& queueAlloc = Allocator::get());

        void queue(StdFunction function);

        void queue(EngineFunction function);

        template <typename T, typename ... TArgs>
        void queue(TArgs&& ... args)
        {
            auto function = mem_new_shared_alloc<T>(mMiscAlloc, std::forward<TArgs>(args) ...);
            queue(function);
        }

        Async queueReturn(StdFunction function);

        Async queueReturn(EngineFunction function);

        template <typename T, typename ... TArgs>
        Async queueReturn(TArgs&& ... args)
        {
            auto function = mem_new_shared_alloc<T>(mMiscAlloc, std::forward<TArgs>(args) ...);
            return queueReturn(function);
        }

        void flush();

        TSharedPtr<CommandQueueBuffer> getSubmitQueue();

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

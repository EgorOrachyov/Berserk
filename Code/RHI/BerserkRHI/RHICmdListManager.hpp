/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHICMDLISTMANAGER_HPP
#define BERSERK_RHICMDLISTMANAGER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Templates/Queue.hpp>
#include <BerserkCore/Threading/CommandBuffer.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {
    namespace RHI {

        /** Manages command buffers with rendering commands, which are submitted for execution to the RHI thread. */
        class CmdListManager {
        public:
            explicit CmdListManager(size_t cmdBufferSizeInBytes = Memory::KiB * 10);
            ~CmdListManager();

            void AllocateCmdBuffer(CommandBuffer* &allocatedBuffer);
            void ReleaseCmdBuffer(CommandBuffer* buffer);
            void SubmitAndAllocateCmdBuffer(CommandBuffer* submittedBuffer, CommandBuffer* &allocatedBuffer);
            bool PopCommandBufferForExecution(CommandBuffer* &buffer);

            void BeginFrame();
            void EndFrame();

            size_t GetCmdBufferMemSize() const;
            size_t GetTotalBuffersCount() const;
            size_t GetAllocatedBuffersCount() const;
            size_t GetPendingExecutionBuffersCount() const;

        protected:
            static const uint32 QUEUE_FIRST  = 0;
            static const uint32 QUEUE_SECOND = 1;
            static const uint32 QUEUE_TOTAL = 2;

            CommandBuffer* AllocateImpl();

            PoolAllocator<> mBuffersPool;
            Array<CommandBuffer*> mCached;
            Queue<CommandBuffer*> mPending[QUEUE_TOTAL];

            size_t mCmdBufferSize;
            uint32 mSubmitQueue = QUEUE_FIRST;
            uint32 mExecQueue = QUEUE_SECOND;

            mutable SpinMutex mMutex;
        };

    }
}

#endif //BERSERK_RHICMDLISTMANAGER_HPP
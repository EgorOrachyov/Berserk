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
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/Containers/Queue.hpp>
#include <BerserkCore/Threading/CommandBuffer.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {
    namespace RHI {

        /** Manages command buffers with rendering commands, which are submitted for execution to the RHI thread. */
        class CmdListManager {
        public:
            explicit CmdListManager(size_t cmdBufferSizeInBytes = Platform::Memory::KiB * 10);
            ~CmdListManager();

            void AllocateCmdBuffer(CommandBuffer* &allocatedBuffer);
            void ReleaseCmdBuffer(CommandBuffer* buffer);
            void SubmitAndAllocateCmdBuffer(CommandBuffer* submittedBuffer, CommandBuffer* &allocatedBuffer);

            size_t GetCmdBufferMemSize() const;
            size_t GetTotalBuffersCount() const;
            size_t GetAllocatedBuffersCount() const;
            size_t GetPendingExecutionBuffersCount() const;

        protected:
            CommandBuffer* AllocateImpl();

            PoolAllocator<> mBuffersPool;
            Array<CommandBuffer*> mCached;
            Queue<CommandBuffer*> mPendingExecution;
            size_t mCmdBufferSize;

            mutable Platform::SpinMutex mMutex;
        };

    }
}

#endif //BERSERK_RHICMDLISTMANAGER_HPP
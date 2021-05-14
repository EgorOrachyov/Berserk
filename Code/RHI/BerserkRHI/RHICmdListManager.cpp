/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkRHI/RHICmdListManager.hpp>
#include <BerserkCore/Defines.hpp>

namespace Berserk {
    namespace RHI {

        CmdListManager::CmdListManager(size_t cmdBufferSizeInBytes)
            : mCmdBufferSize(cmdBufferSizeInBytes),
              mBuffersPool(sizeof(CommandBuffer)) {

            BERSERK_ASSERT(mCmdBufferSize >= Memory::KiB && BERSERK_TEXT("Must be at least 1 KiB of the size"));
        }

        CmdListManager::~CmdListManager() {
            CommandBuffer* pendingExecution;

            for (auto& queue: mPending) {
                while (queue.Pop(pendingExecution)) {
                    pendingExecution->~CommandBuffer();
                    mBuffersPool.Deallocate(pendingExecution);
                }
            }

            for (CommandBuffer* cached: mCached) {
                cached->~CommandBuffer();
                mBuffersPool.Deallocate(cached);
            }

            BERSERK_ASSERT(mBuffersPool.GetAllocatedChunks() == 0);
        }

        void CmdListManager::AllocateCmdBuffer(CommandBuffer *&allocatedBuffer) {
            Guard<SpinMutex> guard(mMutex);
            allocatedBuffer = AllocateImpl();
        }

        void CmdListManager::ReleaseCmdBuffer(CommandBuffer *buffer) {
            Guard<SpinMutex> guard(mMutex);
            mCached.Add(buffer);
        }

        void CmdListManager::SubmitAndAllocateCmdBuffer(CommandBuffer *submittedBuffer, CommandBuffer *&allocatedBuffer) {
            Guard<SpinMutex> guard(mMutex);

            // Submit commands
            mPending[mSubmitQueue].Push(submittedBuffer);
            // Allocate new buffer for producer
            allocatedBuffer = AllocateImpl();
        }

        bool CmdListManager::PopCommandBufferForExecution(CommandBuffer *&buffer) {
            Guard<SpinMutex> guard(mMutex);
            return mPending[mExecQueue].Pop(buffer);
        }

        void CmdListManager::BeginFrame() {
            Guard<SpinMutex> guard(mMutex);
            std::swap(mSubmitQueue, mExecQueue);
        }

        void CmdListManager::EndFrame() {
            // nothing
        }

        size_t CmdListManager::GetCmdBufferMemSize() const {
            Guard<SpinMutex> guard(mMutex);
            return mCmdBufferSize;
        }

        size_t CmdListManager::GetTotalBuffersCount() const {
            Guard<SpinMutex> guard(mMutex);
            return mBuffersPool.GetAllocatedChunks();
        }

        size_t CmdListManager::GetAllocatedBuffersCount() const {
            Guard<SpinMutex> guard(mMutex);
            return mBuffersPool.GetAllocatedChunks() - (mPending[QUEUE_FIRST].GetSize() + mPending[QUEUE_SECOND].GetSize());
        }

        size_t CmdListManager::GetPendingExecutionBuffersCount() const {
            Guard<SpinMutex> guard(mMutex);
            return mPending[QUEUE_FIRST].GetSize() + mPending[QUEUE_SECOND].GetSize();
        }

        CommandBuffer* CmdListManager::AllocateImpl() {
            if (mCached.IsNotEmpty()) {
                return mCached.PopLast();
            }

            return new (mBuffersPool.Allocate(sizeof(CommandBuffer))) CommandBuffer(mCmdBufferSize);
        }

    }
}
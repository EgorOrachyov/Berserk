/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkRHI/RHICmdListManager.hpp>
#include <BerserkCore/Defines.hpp>

namespace Berserk {
    namespace RHI {

        CmdListManager::CmdListManager(size_t cmdBufferSizeInBytes)
            : mCmdBufferSize(cmdBufferSizeInBytes),
              mBuffersPool(sizeof(CommandBuffer)) {

            assert(mCmdBufferSize >= Memory::KiB && BERSERK_TEXT("Must be at least 1 KiB of the size"));
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

            assert(mBuffersPool.GetAllocatedChunks() == 0);
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
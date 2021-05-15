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
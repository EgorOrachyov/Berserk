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

#ifndef BERSERK_GLDEFERREDRESOURCES_HPP
#define BERSERK_GLDEFERREDRESOURCES_HPP

#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Templates/CommandQueue.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {
    namespace RHI {

        /** Manage deferred resources initialization and release on RHI thread */
        class GLDeferredResources {
        public:
            using CmdQueueType = CallbackQueue;

            explicit GLDeferredResources(uint64 cmdBufferSize = Memory::KiB * 100);
            ~GLDeferredResources();

            /** Must be called by RHI thread before any cmd list is processed */
            void BeginFrame();
            /** Executes init queues for RHI resource (before cmd lists) */
            void ExecutePendingInitQueue();
            /** Executes release queues for RHI resource (after cmd lists) */
            void ExecutePendingReleaseQueue();
            /** Must be called after all cmd lists are processed */
            void EndFrame();

            template<typename Callable>
            void SubmitInit(Callable&& callable) {
                Guard<SpinMutex> guard(mMutex); {
                    mSubmitInit->Enqueue(std::forward<Callable>(callable));
                }
            }

            template<typename Callable>
            void SubmitRelease(Callable&& callable) {
                Guard<SpinMutex> guard(mMutex); {
                    mSubmitRelease->Enqueue(std::forward<Callable>(callable));
                }
            }

        private:
            uint64 mCmdBufferSize;

            CmdQueueType* mSubmitInit = nullptr;
            CmdQueueType* mSubmitRelease = nullptr;

            CmdQueueType* mDeferredInit = nullptr;
            CmdQueueType* mDeferredRelease = nullptr;

            mutable SpinMutex mMutex;
        };

    }
}

#endif //BERSERK_GLDEFERREDRESOURCES_HPP
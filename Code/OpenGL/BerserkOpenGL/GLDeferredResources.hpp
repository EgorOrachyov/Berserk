/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLDEFERREDRESOURCES_HPP
#define BERSERK_GLDEFERREDRESOURCES_HPP

#include <BerserkCore/Memory/PoolAllocator.hpp>
#include <BerserkCore/Threading/CommandBuffer.hpp>
#include <BerserkCore/Templates/Queue.hpp>

namespace Berserk {
    namespace RHI {

        /** Manage deferred resources initialization and release on RHI thread */
        class GLDeferredResources {
        public:
            explicit GLDeferredResources(size_t cmdBufferSize = Memory::KiB * 100);
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
            CommandBuffer* Allocate();
            void Release(CommandBuffer* buffer);

            size_t mCmdBufferSize;

            CommandBuffer* mSubmitInit = nullptr;
            CommandBuffer* mSubmitRelease = nullptr;

            CommandBuffer* mDeferredInit = nullptr;
            CommandBuffer* mDeferredRelease = nullptr;

            mutable SpinMutex mMutex;
        };

    }
}

#endif //BERSERK_GLDEFERREDRESOURCES_HPP
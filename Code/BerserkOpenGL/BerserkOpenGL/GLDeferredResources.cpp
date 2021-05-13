/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include  <BerserkOpenGL/GLDeferredResources.hpp>

namespace Berserk {
    namespace RHI {

        GLDeferredResources::GLDeferredResources(size_t cmdBufferSize)
            : mCmdBufferSize(cmdBufferSize) {

            mSubmitInit = Allocate();
            mSubmitRelease = Allocate();
            mDeferredInit = Allocate();
            mDeferredRelease = Allocate();
        }

        GLDeferredResources::~GLDeferredResources() {
            BERSERK_ASSERT(mSubmitInit);
            BERSERK_ASSERT(mSubmitRelease);
            BERSERK_ASSERT(mDeferredInit);
            BERSERK_ASSERT(mDeferredRelease);

            mSubmitInit->Execute();
            mSubmitRelease->Execute();

            Release(mSubmitInit);
            Release(mSubmitRelease);
            Release(mDeferredInit);
            Release(mDeferredRelease);
        }

        void GLDeferredResources::BeginFrame() {
            Guard<SpinMutex> guard(mMutex); {
                std::swap(mSubmitInit, mDeferredInit);
                std::swap(mSubmitRelease, mDeferredRelease);
            }
        }

        void GLDeferredResources::ExecutePendingInitQueue() {
            mDeferredInit->Execute();
        }

        void GLDeferredResources::ExecutePendingReleaseQueue() {
            mDeferredRelease->Execute();
        }

        void GLDeferredResources::EndFrame() {
            mDeferredInit->Clear();
            mDeferredRelease->Clear();
        }

        CommandBuffer* GLDeferredResources::Allocate() {
            return Memory::Make<CommandBuffer>(mCmdBufferSize);
        }

        void GLDeferredResources::Release(CommandBuffer *buffer) {
            Memory::Release(buffer);
        }

    }
}

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>
#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLDriver::GLImpl::GLImpl() {
            GLenum error = glewInit();

            if (error != GLEW_OK) {
                // Ensure, that context was made prior that call
                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Failed to initialize GLEW: \"{0}\""), (const char*) glewGetErrorString(error));
            }

            mSignalStop.store(false);

            mDevice = Platform::Memory::Make<GLDevice>();
            mContext = Platform::Memory::Make<GLContext>();
            mCmdListManager = Platform::Memory::Make<CmdListManager>();
            mDeferredResources = Platform::Memory::Make<GLDeferredResources>();

            Provide(this);

            mThread = Platform::ThreadManager::CreateThread(BERSERK_TEXT("RHI-OPENGL-THREAD"), [this](){
                this->FixedUpdate();
            });
        }

        GLDriver::GLImpl::~GLImpl() {
            mSignalStop.store(true);
            mThread->Join();

            Platform::Memory::Release(mDeferredResources);
            Platform::Memory::Release(mCmdListManager);
            Platform::Memory::Release(mContext);
            Platform::Memory::Release(mDevice);

            Remove(this);
        }

        void GLDriver::GLImpl::FixedUpdate() {
            /*
             * This code is running on RHI thread.
             */

            while (!mSignalStop.load()) {
                mDeferredResources->BeginFrame();
                mCmdListManager->BeginFrame();

                mDeferredResources->ExecutePendingInitQueue();

                // Execute all pending command buffers (from cmd lists)
                {
                    CommandBuffer* mCmdList = nullptr;
                    while (mCmdListManager->PopCommandBufferForExecution(mCmdList)) {
                        mCmdList->Execute();
                        mCmdList->Clear();
                        mCmdListManager->ReleaseCmdBuffer(mCmdList);
                    }
                }

                mDeferredResources->ExecutePendingReleaseQueue();

                mCmdListManager->EndFrame();
                mDeferredResources->EndFrame();
            }
        }

        Device &GLDriver::GLImpl::GetDevice() {
            return *mDevice;
        }

        Context &GLDriver::GLImpl::GetContext() {
            return *mContext;
        }

        CmdListManager &GLDriver::GLImpl::GetCmdListManager() {
            return *mCmdListManager;
        }

        GLDeferredResources & GLDriver::GLImpl::GetDeferredResourceContext() {
            return *mDeferredResources;
        }
    }
}

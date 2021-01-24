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

            mDevice = Platform::Memory::Make<GLDevice>();
            mContext = Platform::Memory::Make<GLContext>();
            mDeferredResources = Platform::Memory::Make<GLDeferredResources>();
            mCmdListManager = Platform::Memory::Make<CmdListManager>();

            Provide(this);

#ifdef BERSERK_TARGET_LINUX
            mSignalStop.store(false);
            mThread = Platform::ThreadManager::CreateThread(BERSERK_TEXT("RHI-OPENGL-THREAD"), [this](){
                /*
                 * This code is running on RHI thread.
                 */

                while (!this->mSignalStop.load()) {
                    this->FixedUpdate();
                }
            });
#endif
        }

        GLDriver::GLImpl::~GLImpl() {
#ifdef BERSERK_TARGET_LINUX
            mSignalStop.store(true);
            mThread->Join();
#endif

            Platform::Memory::Release(mCmdListManager);
            Platform::Memory::Release(mDeferredResources);
            Platform::Memory::Release(mContext);
            Platform::Memory::Release(mDevice);

            Remove(this);
        }

        void GLDriver::GLImpl::FixedUpdate() {
            // Swap queues, pending ops for init or release
            mDeferredResources->BeginFrame();
            // Swap submit and exec queues
            mCmdListManager->BeginFrame();

            // Init all resources. They will be available for all subsequent cmd lists
            mDeferredResources->ExecutePendingInitQueue();

            // Execute all pending command buffers (from cmd lists)
            CommandBuffer* cmdList = nullptr;
            while (mCmdListManager->PopCommandBufferForExecution(cmdList)) {
                cmdList->Execute();
                cmdList->Clear();
                mCmdListManager->ReleaseCmdBuffer(cmdList);
            }

            // Release resources. At this moment nowhere in the system references to these resoruces are presented
            mDeferredResources->ExecutePendingReleaseQueue();

            mCmdListManager->EndFrame();
            mDeferredResources->EndFrame();
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

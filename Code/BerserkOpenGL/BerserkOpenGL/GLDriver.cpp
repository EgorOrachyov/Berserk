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

#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

namespace Berserk {
    namespace RHI {

        GLDriver::GLImpl::GLImpl() {
            GLenum error = glewInit();

            if (error != GLEW_OK) {
                // Ensure, that context was made prior that call
                BERSERK_GL_LOG_ERROR(BERSERK_TEXT("Failed to initialize GLEW: \"{0}\""), (const char*) glewGetErrorString(error));
                return;
            }

            mDevice = Memory::Make<GLDevice>();
            mDeferredResources = Memory::Make<GLDeferredResources>();
            mContext = Memory::Make<GLContext>();
            mCmdListManager = Memory::Make<AsyncCommandQueueConsumer<>>();

            Provide(this);
        }

        GLDriver::GLImpl::~GLImpl() {
            if (IsInitialized()) {
                Memory::Release(mCmdListManager);
                Memory::Release(mContext);
                Memory::Release(mDeferredResources);
                Memory::Release(mDevice);

                Remove(this);
            }
        }

        bool GLDriver::GLImpl::IsInitialized() const {
            return mDevice != nullptr;
        }

        void GLDriver::GLImpl::FixedUpdate() {
            // Swap queues, pending ops for init or release
            mDeferredResources->BeginFrame();

            // Release resources. At this moment nowhere in the system references to these resources are presented
            mDeferredResources->ExecutePendingReleaseQueue();

            // Init all resources. They will be available for all subsequent cmd lists
            mDeferredResources->ExecutePendingInitQueue();

            // Execute pending queues and then swap (so next exec will be what currently is submitted)
            mCmdListManager->ExecutePending();

            // Finish deferred resources scope
            mDeferredResources->EndFrame();

            // Context management (caches update)
            mContext->GC();
        }

        Device &GLDriver::GLImpl::GetDevice() {
            return *mDevice;
        }

        Context &GLDriver::GLImpl::GetContext() {
            return *mContext;
        }

        GLDeferredResources & GLDriver::GLImpl::GetDeferredResourceContext() {
            return *mDeferredResources;
        }

        AsyncCommandQueue<> GLDriver::GLImpl::GetCommandQueue() {
            return mCmdListManager->CreateQueue();
        }
    }
}

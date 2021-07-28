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

#ifndef BERSERK_GLDRIVER_HPP
#define BERSERK_GLDRIVER_HPP

#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkOpenGL/GLDevice.hpp>
#include <BerserkOpenGL/GLContext.hpp>
#include <BerserkOpenGL/GLDeferredResources.hpp>
#include <BerserkCore/Platform/Thread.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief OpenGL driver
         *
         * Driver setups OpenGL RHI implementation backend.
         * Driver runs its update on separate thread if this feature is available by target platform.
         *
         * @note On MacOS with GLFW winodwing the RHI is updated on the main thread,
         *       where the system update happens.
         */
        class GLDriver final: public Driver {
        public:

            class GLImpl final: public Driver::Impl {
            public:
                GLImpl();
                ~GLImpl() override;

                bool IsInitialized() const;
                void FixedUpdate();

                Device &GetDevice() override;
                Context &GetContext() override;
                GLDeferredResources &GetDeferredResourceContext();
                AsyncCommandQueue<> GetCommandQueue();

            private:
                GLDevice* mDevice = nullptr;
                GLContext* mContext = nullptr;
                GLDeferredResources* mDeferredResources = nullptr;
                AsyncCommandQueueConsumer<> *mCmdListManager = nullptr;
            };

            static Device &GetDevice() {
                return GLImpl::Instance().GetDevice();
            }

            static Context &GetContext() {
                return GLImpl::Instance().GetContext();
            }

            static AsyncCommandQueue<> GetCommandQueue() {
                return ((GLImpl&) GLImpl::Instance()).GetCommandQueue();
            }

            static GLDeferredResources &GetDeferredResourceContext() {
                return ((GLImpl&) GLImpl::Instance()).GetDeferredResourceContext();
            }

        };

    }
}


#endif //BERSERK_GLDRIVER_HPP

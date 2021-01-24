/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLDRIVER_HPP
#define BERSERK_GLDRIVER_HPP

#include <BerserkRHI/RHIDriver.hpp>
#include <BerserkRHI/RHICmdListManager.hpp>
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
        class GLDriver: public Driver {
        public:

            class GLImpl final: public Driver::Impl {
            public:
                GLImpl();
                ~GLImpl() override;

                void FixedUpdate();

                Device &GetDevice() override;
                Context &GetContext() override;
                CmdListManager &GetCmdListManager() override;
                GLDeferredResources &GetDeferredResourceContext();

            private:
#ifdef BERSERK_TARGET_LINUX
                /** Driver constantly running in this thread */
                Ref<Platform::Thread> mThread;
                Platform::AtomicBool mSignalStop;
#endif

                GLDevice* mDevice = nullptr;
                GLContext* mContext = nullptr;
                CmdListManager* mCmdListManager = nullptr;
                GLDeferredResources* mDeferredResources = nullptr;
            };

            static Device &GetDevice() {
                return GLImpl::Instance().GetDevice();
            }

            static Context &GetContext() {
                return GLImpl::Instance().GetContext();

            }
            static CmdListManager &GetCmdListManager() {
                return GLImpl::Instance().GetCmdListManager();
            }

            static GLDeferredResources &GetDeferredResourceContext() {
                return ((GLImpl&) GLImpl::Instance()).GetDeferredResourceContext();
            }

        };

    }
}


#endif //BERSERK_GLDRIVER_HPP

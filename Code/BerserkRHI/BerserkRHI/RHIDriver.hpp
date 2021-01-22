
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIDRIVER_HPP
#define BERSERK_RHIDRIVER_HPP

#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkRHI/RHIDevice.hpp>
#include <BerserkRHI/RHIContext.hpp>
#include <BerserkRHI/RHICmdListManager.hpp>

namespace Berserk {
    namespace RHI {

        /** Access RHI globals for graphics application */
        class Driver {
        public:

            /**
             * Access global within the engine RHI device instance.
             *
             * @note Device could be possibly not presented, if the application is created in
             *       non-graphics mode.
             *
             * @return Driver instance reference
             */
            static Device& GetDevice() {
                return Impl::Instance().GetDevice();
            }

            /**
             * Access global within the engine RHI Context instance.
             *
             * @note RHI Context must be accessed only on RHI thread. Other threads must enqueue
             *       its commands via special command list objects.
             *
             * @note Context instance could be possibly not presented, if the application is created in
             *       non-graphics mode and there is no RHI device instance.
             *
             * @return Context instance reference
             */
            static Context& GetContext() {
                return Impl::Instance().GetContext();
            }

        protected:
            friend class CmdList;

            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() = default;
                virtual Device& GetDevice() = 0;
                virtual Context& GetContext() = 0;
                virtual CmdListManager& GetCmdListManager() = 0;
            };

        };

    }
}

#endif //BERSERK_RHIDRIVER_HPP
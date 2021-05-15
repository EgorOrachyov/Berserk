
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

#ifndef BERSERK_RHIDRIVER_HPP
#define BERSERK_RHIDRIVER_HPP

#include <BerserkCore/Templates/Singleton.hpp>
#include <BerserkRHI/RHIDevice.hpp>
#include <BerserkRHI/RHIContext.hpp>

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
            };

        };

    }
}

#endif //BERSERK_RHIDRIVER_HPP
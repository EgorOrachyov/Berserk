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

#ifndef BERSERK_ENTRYPOINT_HPP
#define BERSERK_ENTRYPOINT_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Templates/Singleton.hpp>

namespace Berserk {

    class Application;
    class PlatformSetup;

    /** Platform specific entry point and update for core systems. */
    class EntryPoint {
    protected:
        friend class ::Berserk::Application;
        friend class ::Berserk::PlatformSetup;

        /**
         * Initialize low-level platform systems, such as
         * memory managers, paths, strings, input, threading and etc.
         *
         * Must be called before any engine primitive is used.
         */
        static void PlatformInitialize();

        /**
         * Post initialize call to setup rendering hardware.
         * This call is separate, because RHI setup requires at least one window to be presented in the system.
         *
         * Must be called after PlatformInitialize only for GUI applications.
         */
        static void PlatformInitializeRHI();

        /**
         * Fixed system update, to query system events,
         * process input and gui elements update.
         *
         * Must be called inside main game loop
         */
        static void FixedUpdate();

        /**
         * Finalize global platform specific systems.
         *
         * Must be called before actual application exit, when
         * all engine modules are finalized.
         */
        static void PlatformFinalize();

    };
}

#endif //BERSERK_ENTRYPOINT_HPP

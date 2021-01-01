/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ENTRYPOINT_HPP
#define BERSERK_ENTRYPOINT_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Misc/Singleton.hpp>

namespace Berserk {

    class Application;
    class PlatformSetup;

    namespace Platform {

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
}

#endif //BERSERK_ENTRYPOINT_HPP

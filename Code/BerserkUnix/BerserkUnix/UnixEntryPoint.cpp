/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/EntryPoint.hpp>
#include <BerserkUnix/UnixSystem.hpp>

namespace Berserk {
    namespace Platform {

        static uint8 MemoryBuffer[sizeof(UnixSystem::UnixImpl)];
        static UnixSystem::UnixImpl* Platform = nullptr;

        void EntryPoint::PlatformInitialize() {
            Platform = new ((void *) MemoryBuffer) UnixSystem::UnixImpl();
        }

        void EntryPoint::FixedUpdate() {
            Platform->FixedUpdate();
        }

        void EntryPoint::PlatformFinalize() {
            Platform->~UnixImpl();
            Platform = nullptr;
        }

    }
}
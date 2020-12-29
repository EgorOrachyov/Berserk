/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/EntryPoint.hpp>
#include <BerserkLinux/LinuxSystem.hpp>

namespace Berserk {
    namespace Platform {

        static volatile uint8 MemoryBuffer[sizeof(LinuxSystem::LinuxImpl)];
        static LinuxSystem::LinuxImpl* Platform = nullptr;

        void EntryPoint::PlatformInitialize() {
            Platform = new ((void *) MemoryBuffer) LinuxSystem::LinuxImpl();
        }

        void EntryPoint::FixedUpdate() {
            Platform->FixedUpdate();
        }

        void EntryPoint::PlatformFinalize() {
            Platform->~LinuxImpl();
            Platform = nullptr;
        }

    }
}
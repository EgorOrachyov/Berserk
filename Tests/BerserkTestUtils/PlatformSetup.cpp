/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <PlatformSetup.hpp>
#include <BerserkCore/Platform/EntryPoint.hpp>

namespace Berserk {
    PlatformSetup::PlatformSetup() {
        Platform::EntryPoint::PlatformInitialize();
    }

    PlatformSetup::~PlatformSetup() {
        Platform::EntryPoint::PlatformFinalize();
    }

    void PlatformSetup::FixedUpdate() {
        Platform::EntryPoint::FixedUpdate();
    }

    void PlatformSetup::InitializeRHI() {
        Platform::EntryPoint::PlatformInitializeRHI();
    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <PlatformSetup.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Platform/EntryPoint.hpp>

namespace Berserk {
    PlatformSetup::PlatformSetup() {
        Platform::EntryPoint::PlatformInitialize();
    }

    PlatformSetup::~PlatformSetup() {
        auto allocCalls = Platform::Memory::GetAllocateCalls();
        auto deallocCalls = Platform::Memory::GetDeallocateCalls();

        Platform::EntryPoint::PlatformFinalize();

        printf("Alloc calls=%llu, Dealloc calls=%llu\n", (unsigned long long)allocCalls, (unsigned long long) deallocCalls);
    }
}
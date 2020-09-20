/**********************************************************************************/
/* This file is part of LLGFx project                                             */
/* https://github.com/EgorOrachyov/LLGFx                                          */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2020 Egor Orachyov                                               */
/**********************************************************************************/

#ifndef BERSERK_PLATFORM_HPP
#define BERSERK_PLATFORM_HPP

#include <BerserkCore/BuildOptions.hpp>
#include <BerserkCore/Platform/Typedefs.hpp>

#ifdef BERSERK_TARGET_MACOS
    #include <BerserkCore/Platform/PlatformMacOS.hpp>
#else
    #error "Target was not specified"
#endif

#endif //BERSERK_PLATFORM_HPP
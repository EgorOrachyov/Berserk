/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORM_HPP
#define BERSERK_PLATFORM_HPP

#include <BerserkCore/BuildOptions.hpp>
#include <BerserkCore/Platform/Typedefs.hpp>

#ifdef BERSERK_TARGET_MACOS
    #include <BerserkCore/Platform/PlatformMacOS.hpp>
    #define BERSERK_TARGET_SPECIFIED
#endif

#ifdef BERSERK_TARGET_LINUX
    #include <BerserkCore/Platform/PlatformLinux.hpp>
    #define BERSERK_TARGET_SPECIFIED
#endif

#if !defined(BERSERK_TARGET_SPECIFIED)
    #error "Target was not specified"
#endif

#endif //BERSERK_PLATFORM_HPP
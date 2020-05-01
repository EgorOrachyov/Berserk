/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LOGMACRO_H
#define BERSERK_LOGMACRO_H

#include <Platform/ISystem.h>

#define BERSERK_LOG(verbosity,...)                                                                 \
    do { ISystem::getSingleton().getLog().logf(verbosity, __VA_ARGS__); } while (0);

#define BERSERK_LOG_INFO(...)                                                                      \
    do { ISystem::getSingleton().getLog().logf(ELogVerbosity::Info, __VA_ARGS__); } while (0);

#define BERSERK_LOG_WARNING(...)                                                                   \
    do { ISystem::getSingleton().getLog().logf(ELogVerbosity::Warning, __VA_ARGS__); } while (0);

#define BERSERK_LOG_ERROR(...)                                                                     \
    do { ISystem::getSingleton().getLog().logf(ELogVerbosity::Error, __VA_ARGS__); } while (0);

#endif //BERSERK_LOGMACRO_H

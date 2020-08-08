/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LOGMACRO_H
#define BERSERK_LOGMACRO_H

#include <Platform/System.h>

#define BERSERK_LOG(verbosity,...)                                                                 \
    do { System::getSingleton().getLog().logf(verbosity, __VA_ARGS__); } while (0);

#define BERSERK_LOG_INFO(...)                                                                      \
    do { System::getSingleton().getLog().logf(ELogType::Info, __VA_ARGS__); } while (0);

#define BERSERK_LOG_WARNING(...)                                                                   \
    do { System::getSingleton().getLog().logf(ELogType::Warning, __VA_ARGS__); } while (0);

#define BERSERK_LOG_ERROR(...)                                                                     \
    do { System::getSingleton().getLog().logf(ELogType::Error, __VA_ARGS__); } while (0);

#endif //BERSERK_LOGMACRO_H

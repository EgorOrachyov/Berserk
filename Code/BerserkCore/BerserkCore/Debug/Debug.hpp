/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_DEBUG_HPP
#define BERSERK_DEBUG_HPP

#include <BerserkCore/IO/Log.hpp>

namespace Berserk {

    class Debug {
    public:
        static Log& GetDebugLog();
    };

#define BERSERK_LOG_MESSAGE(verbosity, category, message) \
    Debug::GetDebugLog().LogMessage(category, message, verbosity);

#define BERSERK_LOG_FORMATTED(verbosity, category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, verbosity, __VA_ARGS__);


#define BERSERK_LOG_TO_FILE(category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, Log::Verbosity::Logging, __VA_ARGS__);

#define BERSERK_LOG_INFO(category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, Log::Verbosity::Info, __VA_ARGS__);

#define BERSERK_LOG_WARNING(category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, Log::Verbosity::Warning, __VA_ARGS__);

#define BERSERK_LOG_ERROR(category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, Log::Verbosity::Error, __VA_ARGS__);

#define BERSERK_LOG_FATAL(category, ... ) \
    Debug::GetDebugLog().LogMessageF(category, Log::Verbosity::Fatal, __VA_ARGS__);


#define BERSERK_CORE_LOG_TO_FILE(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_TEXT("Core"), Log::Verbosity::Logging, __VA_ARGS__);

#define BERSERK_CORE_LOG_INFO(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_TEXT("Core"), Log::Verbosity::Info, __VA_ARGS__);

#define BERSERK_CORE_LOG_WARNING(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_TEXT("Core"), Log::Verbosity::Warning, __VA_ARGS__);

#define BERSERK_CORE_LOG_ERROR(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_TEXT("Core"), Log::Verbosity::Error, __VA_ARGS__);

#define BERSERK_CORE_LOG_FATAL(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_TEXT("Core"), Log::Verbosity::Fatal, __VA_ARGS__);

}

#endif //BERSERK_DEBUG_HPP
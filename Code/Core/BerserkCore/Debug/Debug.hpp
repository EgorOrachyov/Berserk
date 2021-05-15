/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_DEBUG_HPP
#define BERSERK_DEBUG_HPP

#include <BerserkCore/IO/Log.hpp>

namespace Berserk {

    class Debug {
    public:
        static Log& GetDebugLog();
    };

#define BERSERK_LOG_CORE BERSERK_TEXT("Core")

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
    Debug::GetDebugLog().LogMessageF(BERSERK_LOG_CORE, Log::Verbosity::Logging, __VA_ARGS__);

#define BERSERK_CORE_LOG_INFO(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_LOG_CORE, Log::Verbosity::Info, __VA_ARGS__);

#define BERSERK_CORE_LOG_WARNING(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_LOG_CORE, Log::Verbosity::Warning, __VA_ARGS__);

#define BERSERK_CORE_LOG_ERROR(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_LOG_CORE, Log::Verbosity::Error, __VA_ARGS__);

#define BERSERK_CORE_LOG_FATAL(...) \
    Debug::GetDebugLog().LogMessageF(BERSERK_LOG_CORE, Log::Verbosity::Fatal, __VA_ARGS__);

}

#endif //BERSERK_DEBUG_HPP
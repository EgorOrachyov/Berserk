/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOG_HPP
#define BERSERK_LOG_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class ELogMessageType {
        Info = 0,
        Warning,
        Error
    };

    class LogListener {
    public:
        virtual ~LogListener() = default;
        virtual void onLog(ELogMessageType messageType, const char* messageTag, const char* message) = 0;
    };

    class Log {
    public:
        virtual ~Log() = default;
        virtual void addListener(LogListener& listener) = 0;
        virtual void removeListener(LogListener& listener) = 0;
        virtual void log(ELogMessageType messageType, const char* messageTag, const char* message) = 0;
        virtual void log(ELogMessageType messageType, const char* messageTag, const char* message, uint32 messageLength) = 0;
    };

}

#endif //BERSERK_LOG_HPP
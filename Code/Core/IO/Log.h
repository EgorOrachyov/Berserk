/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LOG_H
#define BERSERK_LOG_H

#include <Typedefs.h>

namespace Berserk {

    /** Types of the log messages (also used to set log priority, i.e. how to filter messages) */
    enum class ELogType : uint32 {
        NoLogging = 0, /** Disables any logging for a log */
        Info = 1,      /** (Lowest priority) User info or message */
        Warning = 2,   /** Warning message */
        Error = 3,     /** Error message (always mirrored from error macro for System log) */
    };

    /** Generic log messages listener */
    class LogListener {
    public:
        /**
         * Called when the logger gets new log message
         * @param messageType Type of the message to log
         * @param message Actual message as C-style string
         */
        virtual void onLog(ELogType messageType, const char *message) = 0;
    };

    /** Generic logger interface */
    class Log {
    public:
        virtual ~Log() = default;

        /** @return Level of this logger priority */
        virtual ELogType getLogType() const = 0;

        /** @return True if allows to subscribe listeners */
        virtual bool supportsListeners() const { return false; }

        /** Adds log listener */
        virtual void addListener(LogListener& listener) { }

        /** Removes log listener */
        virtual void removeListener(LogListener& listener) { }

        /** Log message */
        virtual void log(ELogType messageType, const char *message) = 0;

        /** Log info message */
        virtual void logInfo(const char* message) { log(ELogType::Info, message); };

        /** Log error message */
        virtual void logError(const char* message) { log(ELogType::Error, message); };

        /** Log warning message */
        virtual void logWarning(const char* message) { log(ELogType::Warning, message); };

        template <typename ... TArgs>
        void logf(ELogType messageType, const char* format, TArgs&& ... args) {
            logfn<2048,TArgs...>(messageType, format, std::forward<TArgs>(args)...);
        }

        template <uint32 SIZE, typename ... TArgs>
        void logfn(ELogType messageType, const char* format, TArgs&& ... args) {
            char buffer[SIZE];
            snprintf(buffer, SIZE, format, std::forward<TArgs>(args)...);
            log(messageType, buffer);
        }

        static const char* getLogTypeStringFromEnum(ELogType verbosity);

    };

}

#endif //BERSERK_LOG_H
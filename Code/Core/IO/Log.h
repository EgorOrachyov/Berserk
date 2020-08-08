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

    enum class ELogType : uint32 {
        NoLogging = 0, /** Disables any logging for a log */
        Info = 1,      /** (Lowest priority) User info or message */
        Warning = 2,   /** Warning message */
        Error = 3,     /** Error message (always mirrored from error macro for System log) */
    };

    class Log {
    public:
        virtual ~Log() = default;

        virtual ELogType getLogType() const = 0;
        virtual void log(ELogType verbosity, const char* message) = 0;
        virtual void logInfo(const char* message) { log(ELogType::Info, message); };
        virtual void logError(const char* message) { log(ELogType::Error, message); };
        virtual void logWarning(const char* message) { log(ELogType::Warning, message); };

        template <typename ... TArgs>
        void logf(ELogType verbosity, const char* format, TArgs&& ... args) {
            const uint32 SIZE = 2048;
            char buffer[SIZE];
            snprintf(buffer, SIZE, format, std::forward<TArgs>(args)... );
            log(verbosity, buffer);
        }

        template <uint32 SIZE, typename ... TArgs>
        void logfn(ELogType verbosity, const char* format, TArgs&& ... args) {
            char buffer[SIZE];
            snprintf(buffer, SIZE, format, std::forward<TArgs>(args)... );
            log(verbosity, buffer);
        }

        static const char* getVerbosityString(ELogType verbosity);

    };

}

#endif //BERSERK_LOG_H
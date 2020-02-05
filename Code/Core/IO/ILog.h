/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ILOG_H
#define BERSERK_ILOG_H

#include <Typedefs.h>
#include <Format.h>

namespace Berserk {

    enum class ELogVerbosity : uint32 {
        NoLogging = 0, /** Disables any logging for a log */
        Info = 1,      /** (Lowest priority) User info or message */
        Warning = 2,   /** Warning message */
        Error = 3,     /** Error message (always mirrored from error macro for System log) */
    };

    class ILog {
    public:
        virtual ~ILog() = default;

        virtual ELogVerbosity getVerbosity() const = 0;
        virtual void log(ELogVerbosity verbosity, const char* message) = 0;
        virtual void logInfo(const char* message) { log(ELogVerbosity::Info, message); };
        virtual void logError(const char* message) { log(ELogVerbosity::Info, message); };
        virtual void logWarning(const char* message) { log(ELogVerbosity::Info, message); };
        static const char* getVerbosityString(ELogVerbosity verbosity);

        template <typename ... TArgs>
        void logf(ELogVerbosity verbosity, const char* format, TArgs&& ... args) {
            const uint32 SIZE = 2048;
            char buffer[SIZE];
            Format::printf(buffer, SIZE, format, std::forward<TArgs>(args)... );
            log(verbosity, buffer);
        }
    };

}

#endif //BERSERK_ILOG_H
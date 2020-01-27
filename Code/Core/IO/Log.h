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
#include <Format.h>

namespace Berserk {

    enum class ELogVerbosity : uint32 {
        Info = 0,      /** (Lowest priority) User info or message */
        Warning = 1,   /** Warning message */
        Error = 2,     /** Error message (always mirrored from error macro for System log) */
        NoLogging = 3  /** Disables any logging for a log */
    };

    class Log {
    public:
        virtual ~Log() = default;

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

    class LogStdout final : public Log {
    public:
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;
    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Error;
    };

}

#endif //BERSERK_LOG_H
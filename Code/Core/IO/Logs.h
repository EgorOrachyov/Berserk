/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LOGS_H
#define BERSERK_LOGS_H

#include <IO/Log.h>
#include <TPtrUnique.h>
#include <Platform/File.h>
#include <Platform/Mutex.h>
#include <Containers/TArray.h>

namespace Berserk {

    /**
     * @brief Std logger
     * Logger to out info in default output stream
     * @note Thread-Safe
     */
    class LogStdout final : public Log {
    public:
        ~LogStdout() override = default;
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;

    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Info;
    };

    /**
     * @brief File logger
     * Logger to out info in some file, accessed via file handle
     * @note Thread-safe
     */
    class LogFile final : public Log {
    public:
        LogFile(TPtrUnique<File>& file);
        ~LogFile() override = default;
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;

    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Info;
        Mutex mAccessMutex;
        TPtrUnique<File> mLogFile;
    };

    /**
     * @brief Composite logger
     * Redirect log info to internal loggers
     * @note Thread-safe
     */
    class LogComposite final: public Log {
    public:
        LogComposite(const std::initializer_list<Log*> &logs);
        ~LogComposite() override = default;
        ELogVerbosity getVerbosity() const override;
        void log(ELogVerbosity verbosity, const char *message) override;
        void addLogger(Log* log);
    private:
        ELogVerbosity mVerbosity = ELogVerbosity::Info;
        Mutex mAccessMutex;
        TArray<Log*> mLoggers;
    };

}

#endif //BERSERK_LOGS_H
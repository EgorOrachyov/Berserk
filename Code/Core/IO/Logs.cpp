/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Logs.h>
#include <Platform/System.h>

namespace Berserk {
    
    ELogVerbosity LogStdout::getVerbosity() const {
        return mVerbosity;
    }

    void LogStdout::log(ELogVerbosity verbosity, const char *message) {
        auto v = (uint32)verbosity;
        auto mv = (uint32)mVerbosity;
        if (v < mv || mVerbosity == ELogVerbosity::NoLogging) return;

        auto time = System::getSingleton().getTime();
        auto timeInfo = time.toString();

        char header[100];
        sprintf(header,"[%19s][%7s] ",timeInfo.data(),getVerbosityString(verbosity));

        printf("%s%s\n", header, message);
    }

    LogFile::LogFile(TPtrUnique<File> &file) {
        if (file.isNull() || !file->isOpen()) {
            BERSERK_ERROR_RET("Log file is invalid")
        }

        mLogFile = std::move(file);

        auto time = System::getSingleton().getTime();
        auto timeInfo = time.toString();

        char initialMessage[1000];
        auto size = sprintf(initialMessage,
                            "BERSERK Device log file\n"
                            "Type [LogFile] opened in [%19s]\n\n",
                            timeInfo.data());

        mLogFile->write(initialMessage, size * sizeof(char));
    }

    ELogVerbosity LogFile::getVerbosity() const {
        return mVerbosity;
    }

    void LogFile::log(ELogVerbosity verbosity, const char *message) {
        auto v = (uint32)verbosity;
        auto mv = (uint32)mVerbosity;
        if (v < mv || mVerbosity == ELogVerbosity::NoLogging) return;

        auto length = CStringUtility::length(message);

        auto time = System::getSingleton().getTime();
        auto timeInfo = time.toString();

        char header[100];
        auto size = sprintf(header,"[%19s][%7s] ",timeInfo.data(),getVerbosityString(verbosity));

        {
            Guard guard(mAccessMutex);
            mLogFile->write(header, size * sizeof(char));
            mLogFile->write(message, length * sizeof(char));
            mLogFile->write("\n", 1 * sizeof(char));
        }
    }

    LogComposite::LogComposite(const std::initializer_list<Log*> &logs) {
        mLoggers.add(logs);
    }

    ELogVerbosity LogComposite::getVerbosity() const {
        return mVerbosity;
    }

    void LogComposite::log(ELogVerbosity verbosity, const char *message) {
        auto v = (uint32)verbosity;
        auto mv = (uint32)mVerbosity;
        if (v < mv || mVerbosity == ELogVerbosity::NoLogging) return;

        {
            Guard guard(mAccessMutex);

            for (auto log: mLoggers) {
                log->log(verbosity, message);
            }
        }
    }

    void LogComposite::addLogger(Log *log) {
        Guard guard(mAccessMutex);
        mLoggers.add(log);
    }
}

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

        auto time = ISystem::getSingleton().getTime();

        char header[100];
        sprintf(header,
                "[%i.%i.%i %2i:%2i:%2i][%7s] ",
                time.year, time.month + 1, time.dayWeek + 1,
                time.hour, time.min, time.sec,
                getVerbosityString(verbosity));

        printf("%s%s\n", header, message);
    }

    LogFile::LogFile(TUnq<IFile> &file) {
        if (file.isNull() || !file->isOpen()) {
            BERSERK_ERROR_RET("Log file is invalid")
        }

        mLogFile = std::move(file);
        mAccessMutex = ISystem::getSingleton().createMutex();

        auto time = ISystem::getSingleton().getTime();
        char timeInfo[100];
        sprintf(timeInfo,"%i.%i.%i %2i:%2i:%2i",
                            time.year, time.month + 1, time.dayMonth + 1,
                            time.hour, time.min, time.sec
        );

        char initialMessage[1000];
        auto size = sprintf(initialMessage,
                "BERSERK Engine log file\n"
                "Type [LogFile] opened in [%s]\n\n", timeInfo);
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
        auto time = ISystem::getSingleton().getTime();

        char header[100];
        auto size = sprintf(
                header,
                "[%i.%i.%i %2i:%2i:%2i][%7s] ",
                time.year, time.month + 1, time.dayWeek + 1,
                time.hour, time.min, time.sec,
                getVerbosityString(verbosity));

        {
            Guard guard(*mAccessMutex);
            mLogFile->write(header, size * sizeof(char));
            mLogFile->write(message, length * sizeof(char));
            mLogFile->write("\n", 1 * sizeof(char));
        }
    }
}

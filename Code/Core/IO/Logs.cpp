/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Logs.h>
#include <Platform/ISystem.h>

namespace Berserk {
    
    ELogVerbosity LogStdout::getVerbosity() const {
        return mVerbosity;
    }

    void LogStdout::log(ELogVerbosity verbosity, const char *message) {
        auto v = (uint32)verbosity;
        auto mv = (uint32)mVerbosity;
        if (v < mv || mVerbosity == ELogVerbosity::NoLogging) return;

        auto time = ISystem::getSingleton().getTime();
        char timeInfo[100];
        sprintf(timeInfo,
                "%i.%i.%i %i:%i:%i",
                time.year, time.month + 1, time.dayMonth + 1,
                time.hour, time.min, time.sec);

        char header[100];
        sprintf(header,
                "[%19s][%7s] ",
                timeInfo,
                getVerbosityString(verbosity));

        printf("%s%s\n", header, message);
    }

    LogFile::LogFile(TPtrUnique<IFile> &file) {
        if (file.isNull() || !file->isOpen()) {
            BERSERK_ERROR_RET("Log file is invalid")
        }

        mLogFile = std::move(file);

        auto time = ISystem::getSingleton().getTime();
        char timeInfo[100];
        sprintf(timeInfo,
                "%i.%i.%i %i:%i:%i",
                time.year, time.month + 1, time.dayMonth + 1,
                time.hour, time.min, time.sec);

        char initialMessage[1000];
        auto size = sprintf(initialMessage,
                            "BERSERK Device log file\n"
                            "Type [LogFile] opened in [%19s]\n\n",
                            timeInfo);

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
        char timeInfo[100];
        sprintf(timeInfo,
                "%i.%i.%i %i:%i:%i",
                time.year, time.month + 1, time.dayMonth + 1,
                time.hour, time.min, time.sec);

        char header[100];
        auto size = sprintf(header,
                            "[%19s][%7s] ",
                            timeInfo,
                            getVerbosityString(verbosity));

        {
            Guard guard(mAccessMutex);
            mLogFile->write(header, size * sizeof(char));
            mLogFile->write(message, length * sizeof(char));
            mLogFile->write("\n", 1 * sizeof(char));
        }
    }
}

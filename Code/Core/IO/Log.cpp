/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Log.h>

namespace Berserk {

    const char* Log::getVerbosityString(ELogVerbosity verbosity) {
        switch (verbosity) {
            case ELogVerbosity::Info:
                return "Info";
            case ELogVerbosity::Warning:
                return "Warning";
            case ELogVerbosity::Error:
                return "Error";
            case ELogVerbosity::NoLogging:
                return "NoLogging";
            default:
                return "";
        }
    }

    ELogVerbosity LogStdout::getVerbosity() const {
        return mVerbosity;
    }

    void LogStdout::log(ELogVerbosity verbosity, const char *message) {
        auto v = (uint32)verbosity;
        auto mv = (uint32)mVerbosity;

        if (v < mv || mVerbosity == ELogVerbosity::NoLogging) return;
        printf("%s", message);
    }
}



/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/ILog.h>

namespace Berserk {

    const char* ILog::getVerbosityString(ELogVerbosity verbosity) {
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

}


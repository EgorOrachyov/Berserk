/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Log.h>

namespace Berserk {

    const char* Log::getVerbosityString(ELogType verbosity) {
        switch (verbosity) {
            case ELogType::Info:
                return "Info";
            case ELogType::Warning:
                return "Warning";
            case ELogType::Error:
                return "Error";
            case ELogType::NoLogging:
                return "NoLogging";
            default:
                return "";
        }
    }

}



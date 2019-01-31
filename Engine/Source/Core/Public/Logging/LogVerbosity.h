//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LOGVERBOSITY_H
#define BERSERK_LOGVERBOSITY_H

#include "Public/Misc/Types.h"

namespace Berserk
{

    enum LogVerbosity : uint8
    {
        /** Not used, does not do anything */
        NoLogging = 0,

        /** Always prints fatal error to the console (and log file) and crashes the application */
        Fatal,

        /** Fatal semantic used only in the DEBUG mode */
        Error,

        /** Prints warning message to the console (and log file) */
        Warning,

        /** Prints message to the console (and log file) */
        /** The most verbose mod of logging */
        Display,

        /**
         * Note, that in the debug and release mode logging properties
         * could be different for previously stated verbosity levels
         */

    };

}

#endif //BERSERK_LOGVERBOSITY_H

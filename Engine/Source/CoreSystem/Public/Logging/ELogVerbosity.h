//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LOGVERBOSITY_H
#define BERSERK_LOGVERBOSITY_H

#include "Misc/Types.h"

namespace Berserk
{

    enum ELogVerbosity : uint8
    {
        /** Does not do anything */
        NoLogging = 0,

        /** Prints message to the console (and log file) */
        /** The most verbose mod of logging */
        Display,

        /** Prints warning message to the console (and log file) */
        Warning,

        /** Fatal semantic used only in the DEBUG mode */
        Error,

        /** Always prints fatal error to the console (and log file) and crashes the application */
        Fatal
    };

}

#endif //BERSERK_LOGVERBOSITY_H

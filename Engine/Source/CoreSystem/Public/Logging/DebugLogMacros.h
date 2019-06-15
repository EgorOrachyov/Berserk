//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_DEBUGLOGMACROS_H
#define BERSERK_DEBUGLOGMACROS_H

#include <Logging/Debug.h>

#ifndef LOG
/** Prints message to the log file without mirroring to the output console */
#define LOG(verbosity, MSG, ...) \
    Berserk::Debug::get().addMessagef(verbosity, false, MSG, ##__VA_ARGS__);
#endif

#endif //BERSERK_DEBUGLOGMACROS_H

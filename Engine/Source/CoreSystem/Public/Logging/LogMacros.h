//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_LOGMACROS_H
#define BERSERK_LOGMACROS_H

#include <Logging/ILogManager.h>

#ifndef ENGINE_LOG
/** Prints message into specified log */
#define ENGINE_LOG(manager, verbosity, mirror, MSG, ...) \
    manager.addMessagef(verbosity, mirror, MSG, ##__VA_ARGS__);
#endif

#endif //BERSERK_LOGMACROS_H

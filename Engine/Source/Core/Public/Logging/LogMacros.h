//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_LOGMACROS_H
#define BERSERK_LOGMACROS_H

#include <Logging/ILogManager.h>

#ifndef ENGINE_LOG
#define ENGINE_LOG(manager, verbosity, mirror, MSG, ...) \
    manager.addMessagef(verbosity, mirror, MSG, ##__VA_ARGS__);
#endif

#ifndef ENGINE_LOG_WARNING
#define ENGINE_LOG_WARNING(manager, mirror, MSG, ...) \
    manager.addMessagef(ELogVerbosity::Warning, mirror, MSG, ##__VA_ARGS__);
#endif

#ifndef ENGINE_LOG_DISPLAY
#define ENGINE_LOG_DISPLAY(manager, mirror, MSG, ...) \
    manager.addMessagef(ELogVerbosity::Display, mirror, MSG, ##__VA_ARGS__);
#endif

#ifndef ENGINE_LOG_ERROR
#define ENGINE_LOG_ERROR(manager, mirror, MSG, ...) \
    manager.addMessagef(ELogVerbosity::Error, mirror, MSG, ##__VA_ARGS__);
#endif

#endif //BERSERK_LOGMACROS_H

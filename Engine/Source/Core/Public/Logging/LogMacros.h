//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LOGMACROS_H
#define BERSERK_LOGMACROS_H

#include "Misc/Buffers.h"
#include "Misc/Compilation.h"
#include "Logging/LogManager.h"

namespace Berserk
{

#if (DEBUG) || (RELEASE)

    /**
     * The error macro for the log manager (will be evolved in debug or release mode)
     * (push message and arguments to be mapped in that mask)
     */
    #define ERROR(MSG, ...)                                                                                         \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1);     \
        LogManager::getSingleton().addMessage(LogVerbosity::Error, buffer2);                                        \
    }

    /**
     * The error macro for the log manager (will be evolved in debug or release mode)
     * (push message and arguments to be mapped in that mask)
     */
    #define FATAL(MSG, ...)                                                                                         \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1);     \
        LogManager::getSingleton().addMessage(LogVerbosity::Fatal, buffer2);                                        \
    }

#else

    #define TYPE_ERROR(MSG, ...)

#endif

#if (DEBUG) || (RELEASE)

    /**
     * The warning macro for the log manager (will be evolved in debug or release mode)
     * (push message and arguments to be mapped in that mask)
     */
    #define WARNING(MSG, ...)                                                                                       \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1);     \
        LogManager::getSingleton().addMessage(LogVerbosity::Warning, buffer2);                                      \
    }

#else

    #define WARNING(MSG, ...)

#endif

#if DEBUG

    /**
     * The info macro for the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define INFO(MSG, ...)                                                                                          \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1);     \
        LogManager::getSingleton().addMessage(LogVerbosity::Display, buffer2);                                      \
    }

#else

    #define INFO(MSG, ...)

#endif

#if DEBUG

    /**
     * The message macro for the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define PUSH(MSG, ...)                                                                                          \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, "\n > %s", buffer1);                                                                         \
        LogManager::getSingleton().addMessage(LogVerbosity::Display, buffer2);                                      \
    }

    /**
     * The message macro for block of info in the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define PUSH_BLOCK(MSG, ...)                                                                                    \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        sprintf(buffer2, " > %s", buffer1);                                                                         \
        LogManager::getSingleton().blockPush(buffer2);                                                              \
    }

    /**
     * Opens block of info in the log manager
     * (MSG and arguments to be mapped in should describe block name and its info)
     */
    #define OPEN_BLOCK(MSG, ...)                                                                                    \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        LogManager::getSingleton().blockOpen(buffer1);                                                              \
    }

    /**
     * Closes block of info in the log manager
     */
    #define CLOSE_BLOCK(MSG, ...)                                                                                   \
    {                                                                                                               \
        sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
        LogManager::getSingleton().blockClose(buffer1);                                                             \
    }

#else

    #define PUSH(MSG, ...)

    #define PUSH_BLOCK(MSG, ...)

    #define OPEN_BLOCK(BLOCK_MSG, ...)

    #define CLOSE_BLOCK()

#endif

}

#endif //BERSERK_LOGMACROS_H

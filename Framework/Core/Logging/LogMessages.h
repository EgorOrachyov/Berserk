//
// Created by Egor Orachyov on 25.06.2018.
//

#ifndef BERSERKENGINE_LOGCOMPILATIONMACROS_H
#define BERSERKENGINE_LOGCOMPILATIONMACROS_H

#include "Misc/Buffers.h"
#include "Misc/Compilation.h"

#include "LogManager.h"

namespace Berserk
{

#if (DEBUG) || (RELEASE)

    /**
     * The error macro for the log manager (will be evolved in debug or release mode)
     * (push message and arguments to be mapped in that mask)
     */
    #define ERROR(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1); \
        globalLogManager.pushMessage(LogMessageType::LMT_ERROR, buffer2); \
    }

#else

    #define TYPE_ERROR(MSG, ...)

#endif

#if (DEBUG) || (RELEASE)

    /**
     * The warning macro for the log manager (will be evolved in debug or release mode)
     * (push message and arguments to be mapped in that mask)
     */
    #define WARNING(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1); \
        globalLogManager.pushMessage(LogMessageType::LMT_WARNING, buffer2); \
    }

#else

    #define WARNING(MSG, ...)

#endif

#if DEBUG

    /**
     * The info macro for the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define INFO(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer1); \
        globalLogManager.pushMessage(LogMessageType::LMT_INFO, buffer2); \
    }

#else

    #define INFO(MSG, ...)

#endif

#if DEBUG

    /**
     * The message macro for the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define PUSH(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, "\n > %s", buffer1); \
        globalLogManager.pushMessage(LogMessageType::LMT_INFO, buffer2); \
    }

    /**
     * The message macro for block of info in the log manager
     * (push message and arguments to be mapped in that mask)
     */
    #define PUSH_BLOCK(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, " > %s", buffer1); \
        globalLogManager.pushMessageBlock(buffer2); \
    }

    /**
     * Opens block of info in the log manager
     * (MSG and arguments to be mapped in should describe block name and its info)
     */
    #define OPEN_BLOCK(MSG, ...) \
    { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        globalLogManager.beginBlock(buffer1); \
    }

    /**
     * Closes block of info in the log manager
     */
    #define CLOSE_BLOCK() \
    { \
        globalLogManager.endBlock(); \
    }

#else

    #define PUSH(MSG, ...)

    #define PUSH_BLOCK(MSG, ...)

    #define OPEN_BLOCK(BLOCK_MSG, ...)

    #define CLOSE_BLOCK()

#endif

} // namespace Berserk

#endif //BERSERKENGINE_LOGCOMPILATIONMACROS_H

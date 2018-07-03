//
// Created by Egor Orachyov on 25.06.2018.
//

#ifndef BERSERKENGINE_LOGCOMPILATIONMACROS_H
#define BERSERKENGINE_LOGCOMPILATIONMACROS_H

#include "../Essential/Common.h"
#include "../Essential/Buffers.h"
#include "../Essential/CompilationFlags.h"

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
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer_one); \
        globalLogManager.pushMessage(LogMessageType::LMT_ERROR, buffer_two); \
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
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer_one); \
        globalLogManager.pushMessage(LogMessageType::LMT_WARNING, buffer_two); \
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
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, "(LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __func__, __FILE__, buffer_one); \
        globalLogManager.pushMessage(LogMessageType::LMT_INFO, buffer_two); \
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
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, "\n > %s", buffer_one); \
        globalLogManager.pushMessage(LogMessageType::LMT_INFO, buffer_two); \
    }

    /**
     * The message macro for block of info in the log manager
     * (push message and arguments to be mapped in that mask)
     */
#define PUSH_BLOCK(MSG, ...) \
    { \
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, " > %s", buffer_one); \
        globalLogManager.pushMessageBlock(buffer_two); \
    }

    /**
     * Opens block of info in the log manager
     * (MSG and arguments to be mapped in should describe block name and its info)
     */
#define OPEN_BLOCK(MSG, ...) \
    { \
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        globalLogManager.beginBlock(buffer_one); \
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

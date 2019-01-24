//
// Created by Egor Orachyov on 22.05.2018.
//

#ifndef BERSERKENGINE_ASSERT_H
#define BERSERKENGINE_ASSERT_H

#include "Misc/Buffers.h"
#include "Misc/Compilation.h"

#include "Logging/LogManager.h"

#if DEBUG

    /**
     * Check the condition and if it is false print the error message
     * with mapped args in that like a mask
     */

    #define ASSERT(condition, MSG, ...) \
    if (condition) { \
    } \
    else { \
        sprintf(buffer1, MSG, ##__VA_ARGS__); \
        sprintf(buffer2, "(LINE %li: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1);\
        globalLogManager.pushMessage(LogMessageType::LMT_ERROR, buffer2); \
        exit(EXIT_FAILURE); \
    }

#else

    #define ASSERT(condition, MSG, ...)

#endif

#endif //BERSERKENGINE_ASSERT_H

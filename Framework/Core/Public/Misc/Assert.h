//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ASSERT_H
#define BERSERK_ASSERT_H

#include "Public/Misc/Buffers.h"
#include "Public/Misc/Compilation.h"
#include "Public/Logging/LogManager.h"

namespace Berserk
{

#if DEBUG

    /**
     * Check the condition and if it is false print the error message
     * with mapped args in that like a mask
     */

    #define ASSERT(condition, MSG, ...)                                                                             \
        if (condition) {                                                                                            \
        }                                                                                                           \
        else {                                                                                                      \
            sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                   \
            sprintf(buffer2, "(LINE %li: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1); \
            LogManager::getSingleton().addMessage(LogVerbosity::Fatal, buffer2);                                    \
            exit(EXIT_FAILURE);                                                                                     \
        }

#else

    #define ASSERT(condition, MSG, ...)

#endif

}

#endif //BERSERK_ASSERT_H

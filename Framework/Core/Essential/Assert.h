//
// Created by Egor Orachyov on 22.05.2018.
//

#ifndef BERSERKENGINE_ASSERT_H
#define BERSERKENGINE_ASSERT_H

#include "CompilationFlags.h"
#include "Buffers.h"

#include "../Logging/LogManager.h"
#include "../Logging/LogProperties.h"

namespace Berserk
{

#if DEBUG

    /**
     * Check the condition and if it is false print the error message
     * with mapped args in that like a mask
     */
#define ASSERT(condition, MSG, ...) \
    if (condition) { \
    } \
    else { \
        sprintf(buffer_one, MSG, ##__VA_ARGS__); \
        sprintf(buffer_two, "(LINE %li: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer_one);\
        globalLogManager.pushMessage(LogMessageType::LMT_ERROR, buffer_two); \
        exit(EXIT_FAILURE); \
    }

#else

#define ASSERT(condition, MSG, ...)

#endif

} // namespace Berserk

#endif //BERSERKENGINE_ASSERT_H

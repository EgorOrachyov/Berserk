//
// Created by Egor Orachyov on 22.05.2018.
//

#ifndef BERSERKENGINE_DEBUGFUNCTIONS_H
#define BERSERKENGINE_DEBUGFUNCTIONS_H

#include "CompilationFlags.h"
#include "../Loggin/LogManager.h"

namespace Berserk
{

#ifdef DEBUG

#define ASSERT(condition, message) \
    if (condition) { \
    } \
    else { \
        Berserk::globalLogManager.PushMessage(Berserk::LMI_ERROR, message, __LINE__, __FILE__, __FUNCTION__); \
        exit(EXIT_FAILURE); \
    }

#else

    ASSERT(condition, message)

#endif

} // namespace Berserk

#endif //BERSERKENGINE_DEBUGFUNCTIONS_H

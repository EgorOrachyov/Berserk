//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_CAST_H
#define BERSERK_CAST_H

#include "Logging/LogMacros.h"

/** Safe static cast with error closing and logging about error */

#ifndef STATIC_CAST
    #define STATIC_CAST(result,source,type) \
        result = static_cast<type>(source); \
        ASSERT(result != nullptr, "Cannot cast statically pointer %p", source);
#endif

/** Safe dynamic cast with error closing and logging about error */

#ifndef DYNAMIC_CAST
    #define DYNAMIC_CAST(result,source,type) \
        result = dynamic_cast<type>(source); \
        if ((result) == nullptr) \
        ASSERT(result != nullptr, "Cannot cast dynamically pointer %p", source);
#endif

#endif //BERSERK_CAST_H

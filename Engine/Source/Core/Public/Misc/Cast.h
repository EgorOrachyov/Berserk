//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_CAST_H
#define BERSERK_CAST_H

/** Safe static cast with error closing and logging about error */

#ifndef STATIC_CAST
    #define STATIC_CAST(result,source,type) \
        result = static_cast<type>(source) \
        if ((result) == nullptr) \
        { \
           /** error log and exit*/ \
        }
#endif

/** Safe dynamic cast with error closing and logging about error */

#ifndef DYNAMIC_CAST
    #define DYNAMIC_CAST(result,source,type) \
        result = dynamic_cast<type>(source) \
        if ((result) == nullptr) \
        { \
           /** error log and exit*/ \
        }
#endif

#endif //BERSERK_CAST_H

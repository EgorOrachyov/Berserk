//
// Created by Egor Orachyov on 10.06.2019.
//

#ifndef BERSERK_ASSERTDEV_H
#define BERSERK_ASSERTDEV_H

#include <cstdio>

namespace Berserk
{

#ifndef ASSERT_DEV
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask.
 *
 * @warning Should be used only for classes with no log assertions
 */
#define ASSERT_DEV(condition)                                                                                           \
        if (condition) {                                                                                                \
        }                                                                                                               \
        else                                                                                                            \
        {   /** Supposed, that 0.5 KiB is enough for buffer to print message*/                                          \
            const unsigned int ____size = 512;                                                                          \
            char ____buffer[____size];                                                                                  \
            snprintf(____buffer, ____size, "(CONDITION: '%s' LINE %i: FUNCTION %s: FILE %s)",                           \
                    #condition, __LINE__, __FUNCTION__, __FILE__);                                                      \
            fprintf(stderr, "%s\n", ____buffer);                                                                        \
            exit(EXIT_FAILURE);                                                                                         \
        }
#endif

} // namespace Berserk

#endif //BERSERK_ASSERTDEV_H

//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_ASSERT_H
#define BERSERK_ASSERT_H

#include <Misc/Include.h>
#include <Misc/Buffers.h>
#include <Misc/Compilation.h>

namespace Berserk
{

#ifndef assert
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask
 */
#define assert(condition)                                                                                               \
        if (condition) {                                                                                                \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            sprintf(buffer1, "(CONDITION: '%s' LINE %i: FUNCTION %s: FILE %s)",                                         \
                    #condition, __LINE__, __FUNCTION__, __FILE__);                                                      \
            fprintf(stderr, "%s\n", buffer2);                                                                           \
            exit(EXIT_FAILURE);                                                                                         \
        }

#endif

#ifndef ASSERT
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask
 */
#define ASSERT(condition, MSG, ...)                                                                                     \
        if (condition) {                                                                                                \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
            sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1); \
            fprintf(stderr, "%s", buffer2);                                                                             \
            exit(EXIT_FAILURE);                                                                                         \
        }

#endif

#ifndef FAIL
/**
 * Check the condition and if it is false print the error message
 * with mapped args in that like a mask. Will crash system in both DEBUG and RELEASE mode
 */
#define FAIL(condition, MSG, ...)                                                                                       \
        if (condition) {                                                                                                \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            sprintf(buffer1, MSG, ##__VA_ARGS__);                                                                       \
            sprintf(buffer2, "\n > (LINE %i: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1); \
            fprintf(stderr, "%s", buffer2);                                                                             \
            exit(EXIT_FAILURE);                                                                                         \
        }
#endif

}

#endif //BERSERK_ASSERT_H

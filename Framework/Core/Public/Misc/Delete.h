//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_DELETE_H
#define BERSERK_DELETE_H

/**
 * Safe delete macro on top of default delete c++ operator,
 * which check, whether the pointer is not null and deletes it and
 * sets its value in null
 */

#ifndef SAFE_DELETE
    #define SAFE_DELETE(pointer) \
        if (pointer) { delete (pointer); (pointer) = nullptr; } else {};
#endif

#endif //BERSERK_DELETE_H

//
// Created by Egor Orachyov on 19.10.2018.
//

#ifndef BERSERK_DELETE_H
#define BERSERK_DELETE_H

#include <cstddef>

#ifndef SAFE_DELETE
    #define SAFE_DELETE(pointer) if (pointer) { delete (pointer); (pointer) = NULL; } else {};
#endif

#include <clocale>

#endif //BERSERK_DELETE_H

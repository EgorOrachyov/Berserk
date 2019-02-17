//
// Created by Egor Orachyov on 17.02.2019.
//

#ifndef BERSERK_OPERATORS_H
#define BERSERK_OPERATORS_H

#include <cstddef>

namespace Berserk
{

    class Operator final
    {
    public:

        /** Overloaded new for creating object on top of the allocated data */
        static void* operator new (size_t size, void* ptr)
        {
            return ptr;
        }

        /** Overloaded delete for objects, created on top of the not-allocatable data */
        static void operator delete (void* ptr)
        {
            return;
        }

    };

} // namespace Berserk

#endif //BERSERK_OPERATORS_H

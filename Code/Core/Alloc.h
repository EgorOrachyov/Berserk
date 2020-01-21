/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ALLOC_H
#define BERSERK_ALLOC_H

#include <Typedefs.h>

namespace Berserk {

    /** Generic allocator interface */
    class Alloc {
    public:
        virtual ~Alloc() = default;
        virtual void* allocate(uint64 size) = 0;
        virtual void free(void* memory) = 0;

        /** @return Default engine allocator (wrapper for C malloc/free) */
        static Alloc& getSingleton();
    };

}

#endif //BERSERK_ALLOC_H
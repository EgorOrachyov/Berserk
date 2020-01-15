/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ATOMIC_H
#define BERSERK_ATOMIC_H

#include <Typedefs.h>

namespace Berserk {

    class Atomic {
    public:
        virtual ~Atomic() = default;
        virtual int32 load() = 0;
        virtual void store(int32 value) = 0;
        virtual int32 add(int32 value) = 0;
        virtual int32 sub(int32 value) = 0;
        virtual bool cas(int32 expected, int32 desired) = 0;
    };

}

#endif //BERSERK_ATOMIC_H
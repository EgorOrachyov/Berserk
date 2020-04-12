/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IATOMIC_H
#define BERSERK_IATOMIC_H

#include <Typedefs.h>

namespace Berserk {

    class IAtomic {
    public:
        virtual ~IAtomic() = default;
        virtual int32 load() = 0;
        virtual void store(int32 value) = 0;
        virtual int32 add(int32 value) = 0;
        virtual int32 sub(int32 value) = 0;
        virtual bool cas(int32 expected, int32 desired) = 0;
    };

}

#endif //BERSERK_IATOMIC_H
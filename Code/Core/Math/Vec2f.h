/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VEC2F_H
#define BERSERK_VEC2F_H

#include <Math/TVecN.h>

namespace Berserk {
    class Vec2f : public TVecN<float32,2> {
    public:
        using TVecN<float32,2>::TVecN;
        using TVecN<float32,2>::operator=;
    };
}


#endif //BERSERK_VEC2F_H
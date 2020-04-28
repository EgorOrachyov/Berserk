/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VEC3F_H
#define BERSERK_VEC3F_H

#include <Math/TVecN.h>

namespace Berserk {

    class Vec3f : public TVecN<float, 3> {
    public:
        using TVecN<float, 3>::TVecN;
        using TVecN<float, 3>::operator=;

        Vec3f(const TVecN<float, 3> &v) : TVecN<float, 3>(v) { }

        Vec3f(float x, float y, float z) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }

        static const Vec3f X_AXIS;
        static const Vec3f Y_AXIS;
        static const Vec3f Z_AXIS;
    };

}

#endif //BERSERK_VEC3F_H
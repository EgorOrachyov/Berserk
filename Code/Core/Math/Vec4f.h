/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VEC4F_H
#define BERSERK_VEC4F_H

#include <Math/TVecN.h>

namespace Berserk {

    class Vec4f : public TVecN<float, 4> {
    public:
        using TVecN<float, 4>::TVecN;
        using TVecN<float, 4>::operator=;

        Vec4f(const TVecN<float,4>& v) : TVecN<float,4>(v) {}

        Vec4f(float x, float y, float z, float w) {
            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
        }

        Vec4f(const TVecN<float, 3> &v, float w) {
            values[0] = v.values[0];
            values[1] = v.values[1];
            values[2] = v.values[2];
            values[3] = w;
        }

        static const Vec4f X_AXIS;
        static const Vec4f Y_AXIS;
        static const Vec4f Z_AXIS;
        static const Vec4f W_AXIS;
    };

}

#endif //BERSERK_VEC4F_H

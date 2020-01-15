/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VEC4F_H
#define BERSERK_VEC4F_H

#include <Math/TVectorN.h>

namespace Berserk {

    class Vec4f : public TVectorN<float32, 4> {
    public:
        using TVectorN<float32, 4>::TVectorN;

        static const Vec4f X_AXIS;
        static const Vec4f Y_AXIS;
        static const Vec4f Z_AXIS;
        static const Vec4f W_AXIS;
    };

    const Vec4f Vec4f::X_AXIS = { 1.0f , 0.0f , 0.0f, 0.0f };
    const Vec4f Vec4f::Y_AXIS = { 0.0f , 1.0f , 0.0f, 0.0f };
    const Vec4f Vec4f::Z_AXIS = { 0.0f , 0.0f , 1.0f, 0.0f };
    const Vec4f Vec4f::W_AXIS = { 0.0f , 0.0f , 0.0f, 1.0f };
}

#endif //BERSERK_VEC4F_H

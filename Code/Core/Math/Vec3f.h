/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VEC3F_H
#define BERSERK_VEC3F_H

#include <Math/TVectorN.h>

namespace Berserk {

    class Vec3f : public TVectorN<float32, 3> {
    public:
        using TVectorN<float32, 3>::TVectorN;

        static const Vec3f X_AXIS;
        static const Vec3f Y_AXIS;
        static const Vec3f Z_AXIS;
    };

    const Vec3f Vec3f::X_AXIS = { 1.0f , 0.0f , 0.0f };
    const Vec3f Vec3f::Y_AXIS = { 0.0f , 1.0f , 0.0f };
    const Vec3f Vec3f::Z_AXIS = { 0.0f , 0.0f , 1.0f };
}

#endif //BERSERK_VEC3F_H
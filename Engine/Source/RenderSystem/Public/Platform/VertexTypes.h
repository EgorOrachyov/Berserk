//
// Created by Egor Orachyov on 16.02.2019.
//

#ifndef BERSERK_VERTEXTYPES_H
#define BERSERK_VERTEXTYPES_H

#include "Math/MathInclude.h"

namespace Berserk
{

    struct Vertf
    {
        Vec3f position;
    };

    struct VertPNf
    {
        Vec3f position;
        Vec3f normal;
    };

    struct VertPTf
    {
        Vec3f position;
        Vec2f texcoords;
    };

    struct VertPNTf
    {
        Vec3f position;
        Vec3f normal;
        Vec2f texcoords;
    };

    struct VertPNTBTf
    {
        Vec3f position;
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
        Vec2f texcoords;
    };

} // namespace Berserk

#endif //BERSERK_VERTEXTYPES_H

//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_RENDERSCENEOBJECTS_H
#define BERSERK_RENDERSCENEOBJECTS_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    class RENDER_API DirLightSceneInfo final : public Allocatable
    {
    public:

        DirLightSceneInfo(
                const Mat4x4f& localToWorld,
                const Vec3f& worldDirection,
                const Vec3f& lightColor,
                bool castShadows,
                float32 distance)

                : LocalToWorld(localToWorld),
                  WorldDirection(worldDirection),
                  LightColor(lightColor),
                  CastShadows(castShadows),
                  Distance(distance)
        {

        }

        Frustum ViewFrustum;
        Mat4x4f LocalToWorld;
        Vec3f WorldDirection;
        Vec3f LightColor;
        bool CastShadows;
        float32 Distance;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENEOBJECTS_H

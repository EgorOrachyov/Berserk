//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_RENDERSCENEINFO_H
#define BERSERK_RENDERSCENEINFO_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>
#include <Rendering/Renderable.h>

namespace Berserk
{

    struct RENDER_API LightSceneInfo : public Allocatable
    {
    public:

        Mat4x4f localToWorld;
        Vec3f lightColor;
        bool castShadows;
        bool isActive;
        float32 distanceOfAction;
        float32 distanceOfActionSq;

    };

    struct RENDER_API DirLightSceneInfo final : public LightSceneInfo
    {
    public:

        Frustum viewFrustum;
        Vec3f worldDirection;

    };

    struct RENDER_API RenderableSceneInfo : public Allocatable
    {
    public:

        Mat4x4f localToWorld;
        TSharedPtr<Renderable> renderable;
        bool castShadows;
        bool applyCulling;
        bool drawWireframe;
        bool drawWireframeOnly;
        bool drawBoundingVolume;
        bool isActive;

    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENEINFO_H

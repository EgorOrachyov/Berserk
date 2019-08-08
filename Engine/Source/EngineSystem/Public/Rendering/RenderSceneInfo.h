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

    /**
     * Base light source info on the scene [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API LightSceneInfo : public Allocatable
    {
    public:

        Mat4x4f localToWorld;
        Vec3f lightColor;
        bool castShadows;
        bool isActive;
        float32 distanceOfAction;
        float32 distanceOfActionSq;

        static void output(const LightSceneInfo &info, IOutputDevice &device)
        {
            device.printf("Color: %s\nCast shadows: %u\nIs active: %u\nDistance of action: %f\n",
                           info.lightColor.toString().get(),
                           info.castShadows,
                           info.isActive,
                           info.distanceOfAction);
        }

    };

    /**
     * Directional light render scene info [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API DirLightSceneInfo final : public LightSceneInfo
    {
    public:

        Frustum viewFrustum;
        Vec3f worldDirection;

        static void output(const DirLightSceneInfo &info, IOutputDevice &device)
        {
            LightSceneInfo::output(info, device);
            device.printf("Direction: %s\n", info.worldDirection.toString().get());
        }

    };

    /**
     * Base renderable obect info (model/mesh) [render thread]
     * @note Each render object must provide scene info
     */
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

        static void output(const RenderableSceneInfo& info, IOutputDevice &device)
        {
            device.printf("Cast shadows: %u\nApply culling: %u\nDraw wireframe: %u\n"
                          "Draw wireframe only: %u\nDraw bounding volume: %u\nIs active: %u\n",
                          info.castShadows,
                          info.applyCulling,
                          info.drawWireframe,
                          info.drawWireframeOnly,
                          info.drawBoundingVolume,
                          info.isActive);
        }

    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENEINFO_H

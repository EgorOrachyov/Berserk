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

    };

    /**
     * Base renderable object info (model/mesh) [render thread]
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

    };

    /**
     * Default camera info for any view on the render scene [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API CameraSceneInfo : public Allocatable
    {
    public:

        Frustum frustum;
        Mat4x4f projection;
        Mat4x4f view;
        Vec3f position;
        Vec3f direction;
        Vec3f up;
        bool useAutoViewport;

        // todo: different optical and cinematic settings support
        // todo: support HDR, depth of field, lens flares

    };

    class DEBUG_API SceneInfo final
    {
    public:

        static void output(const LightSceneInfo &info, IOutputDevice &device)
        {
            device.printf("Color: %s\nCast shadows: %u\nIs active: %u\nDistance of action: %f\n",
                          info.lightColor.toString().get(),
                          info.castShadows,
                          info.isActive,
                          info.distanceOfAction);
        }

        static void output(const DirLightSceneInfo &info, IOutputDevice &device)
        {
            output((LightSceneInfo&) info, device);
            device.printf("Direction: %s\n", info.worldDirection.toString().get());
        }

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

        static void output(const CameraSceneInfo& info, IOutputDevice &device)
        {
            device.printf("Position: %s\nDirection: %s\nUp: %s\nAuto viewport: %u\n",
                          info.position.toString().get(),
                          info.direction.toString().get(),
                          info.up.toString().get(),
                          info.useAutoViewport);
        }
    };

} // namespace Berserk

#endif //BERSERK_RENDERSCENEINFO_H

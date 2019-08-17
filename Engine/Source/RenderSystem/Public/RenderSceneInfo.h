//
// Created by Egor Orachyov on 2019-08-06.
//

#ifndef BERSERK_RENDERSCENEINFO_H
#define BERSERK_RENDERSCENEINFO_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>
#include <Rendering/Renderable.h>
#include <RHI/RHIResources.h>
#include <Rendering/RenderSceneBase.h>
#include <Rendering/RenderElement.h>
#include <Rendering/RenderingCommon.h>
#include <Application/IWindow.h>

namespace Berserk
{

    /**
     * Base light source info on the scene [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API LightSceneInfo : public Allocatable, public RenderSceneInfo
    {
    public:

        ~LightSceneInfo() override = default;

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

        ~DirLightSceneInfo() override = default;

        Frustum viewFrustum;
        Vec3f worldDirection;

    };

    typedef TSharedPtr<DirLightSceneInfo> DirLightSceneInfoRef;

    /**
     * Base renderable object info (model/mesh) [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API RenderableSceneInfo : public RenderSceneInfo
    {
    public:

        ~RenderableSceneInfo() override = default;

        TSharedPtr<Renderable> renderable;
        RHIGeometryBufferRef geometry;

        // todo: manager memory allocation
        TArray<RenderElement> renderElements;
        Mat4x4f localToWorld;

        bool castShadows;
        bool applyCulling;
        bool drawWireframe;
        bool drawWireframeOnly;
        bool drawBoundingVolume;
        bool isActive;

    };

    typedef TSharedPtr<RenderableSceneInfo> RenderableSceneInfoRef;

    /**
     * Default camera info for any view on the render scene [render thread]
     * @note Each render object must provide scene info
     */
    struct RENDER_API CameraSceneInfo : public RenderSceneInfo
    {
    public:

        ~CameraSceneInfo() override = default;

        Frustum frustum;
        Mat4x4f projection;
        Mat4x4f view;

        Rect vieport;
        WindowRef window;

        Vec3f position;
        Vec3f direction;
        Vec3f up;

        bool useAutoViewport;
        bool isActive;

        // todo: different optical and cinematic settings support
        // todo: support HDR, depth of field, lens flares

    };

    typedef TSharedPtr<CameraSceneInfo> CameraSceneInfoRef;

    /** Used for debug info output for each scene info class */
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
                          "Draw wireframe only: %u\nDraw bounding volume: %u\nIs active: %u\nNum of nodes: %u\n",
                          info.castShadows,
                          info.applyCulling,
                          info.drawWireframe,
                          info.drawWireframeOnly,
                          info.drawBoundingVolume,
                          info.isActive,
                          info.renderElements.getSize());
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

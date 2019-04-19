//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_RENDERPASSINFO_H
#define BERSERK_RENDERPASSINFO_H

#include <Foundation/IObjectMacros.h>
#include <Managers/IBufferManager.h>

namespace Berserk::Render
{
    using namespace Resources;

    /**
     * All the information, need for pipeline stages to properly render
     * primitives in current state. Involves info about frame buffers, settings,
     * dimensions, resolution, active resource time and etc.
     *
     * Also defines any kind of action[s], which should be done by pipeline stage:
     * profiling, capturing buffer data, creating a screen shot and etc.
     */
    class RenderPassInfo
    {
    public:

        /** Actually nothing */
        RenderPassInfo() = default;

        /** Actually nothing */
        ~RenderPassInfo() = default;

    public:

        friend class RenderSystem;

        /** Primary frame buffer of downscaled size for rendering [stage input] */
        DECLARE_PROPERTY(FrameBufferIn, IFrameBuffer*, nullptr);

        /** Primary frame buffer of downscaled size for rendering [stage output] */
        DECLARE_PROPERTY(FrameBufferOut, IFrameBuffer*, nullptr);

        /** View port for rendering in main frame buffer */
        DECLARE_PROPERTY(FrameBufferViewPort, IRenderDriver::ViewPort, IRenderDriver::ViewPort());

        /** View port for rendering in main window */
        DECLARE_PROPERTY(DisplayViewPort, IRenderDriver::ViewPort, IRenderDriver::ViewPort());

    public:

        /** Make a screen show for all gen shadow maps */
        DECLARE_PROPERTY(CaptureShadowMaps, bool, false);

        /** Make a screen show for all geometry buffer layers */
        DECLARE_PROPERTY(CaptureDeferredBuffer, bool, false);

        /** Make a screen show for deferred buffer after lightning was applied */
        DECLARE_PROPERTY(CaptureDeferredLightning, bool, false);

        /** Make a screen show for gen SSAO buffer */
        DECLARE_PROPERTY(CaptureSSAOBuffer, bool, false);

        /** Make a screen show for sampled light shafts buffer */
        DECLARE_PROPERTY(CaptureLightShaftBuffer, bool, false);

        /** Make a screen show for light shafts buffer after radial blur */
        DECLARE_PROPERTY(CaptureLightShaftBufferBlured, bool, false);

        /** Make a screen show for sampled bloom buffer */
        DECLARE_PROPERTY(CaptureBloomSampling, bool, false);

        /** Make a screen show for bloom buffer after blur was applied */
        DECLARE_PROPERTY(CaptureBloomBlured, bool, false);

        /** Make a screen show for complete display image */
        DECLARE_PROPERTY(CaptureResultImage, bool, false);

    };

} // namespace Berserk::RenderSystems

#endif //BERSERK_RENDERPASSINFO_H

//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_IRENDERSYSTEM_H
#define BERSERK_IRENDERSYSTEM_H

#include <Systems/ISystem.h>
#include <Base/RenderBase.h>

namespace Berserk::RenderSystem
{
    using namespace EntitySystem;

    class EffectInfoShadowMapping
    {

    };

    class EffectInfoDeferred
    {

    };

    class EffectInfoSSAO
    {

    };

    class EffectInfoLightShafts
    {

    };

    class EffectInfoVolumetricLight
    {

    };

    class EffectInfoBloom
    {

    };

    class EffectInfoToneMapping
    {

    };

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

        /** Primary frame buffer of downscaled size for rendering */
        DECLARE_PROPERTY(MainFrameBuffer, IFrameBuffer*, nullptr);

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

    /**
     * Interface for the rendering system of the engine, Provides
     * exhaustive functionality and initializes its subsystems.
     *
     * Allows to register components, process them, capture screens.
     * Also allows to configure all the effects, available in the rendering engine
     */
    class IRenderSystem : public ISystem, public RenderBase
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(IRenderSystem);

        /** Do actually nothing */
        explicit IRenderSystem(const ISystemInitializer& systemInitializer) : ISystem(systemInitializer) {}

        /** Do actually nothing */
        ~IRenderSystem() override = default;

    public:

        /** Info about shadow mapping */
        DECLARE_PROPERTY_READ(EffectInfoShadowMapping, EffectInfoShadowMapping);

        /** Info about deferred lightning */
        DECLARE_PROPERTY_READ(EffectInfoDeferred, EffectInfoDeferred);

        /** Info about SSAO effect */
        DECLARE_PROPERTY_READ(EffectInfoSSAO, EffectInfoSSAO);

        /** Info about light shafts effect */
        DECLARE_PROPERTY_READ(EffectInfoLightShafts, EffectInfoLightShafts);

        /** Info about volumetric light effect */
        DECLARE_PROPERTY_READ(EffectInfoVolumetricLight, EffectInfoVolumetricLight);

        /** Info about bloom effect */
        DECLARE_PROPERTY_READ(EffectInfoBloom, EffectInfoBloom);

        /** Info about final tone mapping */
        DECLARE_PROPERTY_READ(EffectInfoToneMapping, EffectInfoToneMapping);

    };

} // namespace Berserk

#endif //BERSERK_IRENDERSYSTEM_H

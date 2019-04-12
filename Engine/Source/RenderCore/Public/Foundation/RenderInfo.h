//
// Created by Egor Orachyov on 05.04.2019.
//

#ifndef BERSERK_RENDERINFO_H
#define BERSERK_RENDERINFO_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk::Render
{

    class ENGINE_API RenderInfoShadowMapping
    {
    public:

    };

    class ENGINE_API RenderInfoDeferred
    {

    };

    class ENGINE_API RenderInfoSSAO
    {
    public:

    };

    class ENGINE_API RenderInfoLightShafts
    {
    public:

    };

    class ENGINE_API RenderInfoVolumetricLight
    {
    public:

    };

    class ENGINE_API RenderInfoBloom
    {
    public:

    };

    /**
     * Tone mapping for HDR render buffers: allows to map colors
     * of the scene in the chosen interval.
     */
    class ENGINE_API RenderInfoToneMapping
    {
    public:

        /** Maps high dynamic range colors to default RGBA 8 bits per chanel format */
        float32 Exposure = 3.0f;

        /** Proper color visualization for different displays */
        float32 GammaCorrection = 2.2f;

    };

    /**
     * Distant fog effect. Allows to hide objects, which placed to far
     * away from main camera point of view. Uses the following equation
     * to calculate visibility param to mix colors of the objects.
     *
     *  visibility = e ^ ( -(distance x density) ^ gradient )
     *
     *  result = mix(background,color,visibility)
     *
     *  // or result = (1 - visibility) * background + visibility * color
     *
     */
    class ENGINE_API RenderInfoDistantFog
    {
    public:

        /** Shows how near/far fog begins to affect on scenes objects */
        float32 Density = 0.001;

        /** Shows how fast objects in the fog will be fully hidden */
        float32 Gradient = 2.0f;

        /** Distant fog color (will be blended with background color) */
        Vec4f FogColor = Vec4f(0.0f);

    };

} // namespace Berserk::RenderSystem

#endif //BERSERK_RENDERINFO_H

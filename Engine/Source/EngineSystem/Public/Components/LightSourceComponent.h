//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_LIGHTSOURCECOMPONENT_H
#define BERSERK_LIGHTSOURCECOMPONENT_H

#include <Foundation/RenderBase.h>
#include <Components/SceneComponent.h>

namespace Berserk::Engine
{
    using namespace Resources;

    using namespace Render;

    /**
     * Base component class for creating any kind of light sources in the engine.
     */
    class ENGINE_API LightSourceComponent : public RenderBase, public SceneComponent
    {
    public:

        enum LightSourceType
        {
            eLST_NOT_LIGHT_SOURCE = 0,
            eLST_SPOT_LIGHT,
            eLST_POINT_LIGHT,
            eLST_DIRECTIONAL_LIGHT,

            eLST_TOTAL_TYPES
        };

    public:

        /** Default object setup via initializer */
        explicit LightSourceComponent(const IObjectInitializer& objectInitializer)
                : SceneComponent(objectInitializer),
                  mLightSourceType(eLST_NOT_LIGHT_SOURCE),
                  mCastShadows(FIELD_OFF),
                  mHasLensOpticsEffect(FIELD_OFF),
                  mHasLightShaftsEffect(FIELD_OFF),
                  mHasVolumetricLightEffect(FIELD_OFF)
        {

        }

        /** Do actually nothing */
        ~LightSourceComponent() override = default;

    public:

        /** @return Type of this light source */
        LightSourceType getLightSourceType() const { return mLightSourceType; }

        /** @return True if that light casts shadow */
        bool castShadows() const                { return mCastShadows; }

        /** @return True if can apply to this lens optics effect */
        bool hasLensOpticsEffect() const        { return mHasLensOpticsEffect; }

        /** @return True if can apply to this Light Shafts effect (God's rays) */
        bool hasLightShaftsEffect() const       { return mHasLensOpticsEffect; }

        /** @return True if can apply volumetric light (ray tracing) effect */
        bool hasVolumetricLightEffect() const   { return mHasVolumetricLightEffect; }

        /** @return Distance of action */
        float32 getMaxLightDistance() const     { return mLightMaxDistance; }

        /** @return Depth map test bias */
        float32 getShadowMapBias() const        { return mShadowMapBias; }

        /** @return Shadow map buffer size */
        uint32 getShadowMapSize() const         { return mShadowMapSize; }

    protected:

        friend class ::Berserk::Render::RenderSystem;

        static const uint32 DEFAULT_SHADOW_MAP_SIZE = 512;

        static const float32 DEFAULT_SHADOW_MAP_BIAS;

        /** Type of this light source */
        LightSourceType mLightSourceType;

        /** Color of the light source */
        Vec4f mLightColor = Vec4f(1.0f,1.0f,1.0f,1.0f);

        /** Its position in local space [default (0,0,0,0)] */
        Vec4f mLightPosition;

        /** Shows if that light casts shadow */
        bool mCastShadows : 1;

        /** If can apply to this lens optics effect */
        bool mHasLensOpticsEffect : 1;

        /** If can apply to this Light Shafts effect (God's rays) */
        bool mHasLightShaftsEffect : 1;

        /** If can apply volumetric light (ray tracing) effect */
        bool mHasVolumetricLightEffect : 1;

        /** Distance of action */
        float32 mLightMaxDistance = 1.0f;

        /** Distance of action (squared) */
        float32 mLightMaxDistanceSq = 1.0f;

        /** Control depth map test mistake */
        float32 mShadowMapBias = DEFAULT_SHADOW_MAP_BIAS;

        /** Shadow map buffer size */
        uint32 mShadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

        /** Pointer to relevant shadow map buffer */
        class IDepthBuffer* mShadowMap = nullptr;

        /** Previous registered component in render system  */
        class LightSourceComponent* mPrev = nullptr;

        /** Next registered component in render system  */
        class LightSourceComponent* mNext = nullptr;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_LIGHTSOURCECOMPONENT_H
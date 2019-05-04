//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_LIGHTCOMPONENT_H
#define BERSERK_LIGHTCOMPONENT_H

#include <Foundation/RenderBase.h>
#include <Components/SceneComponent.h>

namespace Berserk::Engine
{
    using namespace Resources;

    using namespace Render;

    /**
     * Base component class for creating any kind of light sources in the engine.
     */
    class ENGINE_API LightComponent : public RenderBase, public SceneComponent
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

        GENERATE_CLASS_BODY(LightComponent);

        /** Default object setup via initializer */
        explicit LightComponent(const IObjectInitializer& objectInitializer)
                : SceneComponent(objectInitializer),
                  mLightSourceType(eLST_NOT_LIGHT_SOURCE),
                  mCastShadows(FIELD_OFF),
                  mHasLensOpticsEffect(FIELD_OFF),
                  mHasLightShaftsEffect(FIELD_OFF),
                  mHasVolumetricLightEffect(FIELD_OFF)
        {

        }

        /** Do actually nothing */
        ~LightComponent() override = default;

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

    public:

        /** @return Color of this source */
        const Vec3f& getLightColor() const      { return mLightColor; }

        /** @return Relative position point */
        const Vec3f& getLocalPosition() const   { return mLocalPosition; }

        /** @return Absolute position point */
        const Vec3f& getWorldPosition() const   { return mWorldPosition; }

        /** @return Distance of action */
        float32 getMaxLightDistance() const     { return mLightMaxDistance; }

        /** @return Depth map test bias */
        float32 getShadowMapBias() const        { return mShadowMapBias; }

        /** @return Shadow map buffer size */
        uint32 getShadowMapSize() const         { return mShadowMapSize; }

    protected:

        friend class ::Berserk::Render::RenderSystem;

        /** Default shadow map size in pixels */
        static const uint32 DEFAULT_SHADOW_MAP_SIZE = 512;

        /** Default bias to avoid depth fighting */
        static const float32 DEFAULT_SHADOW_MAP_BIAS;

        /** Type of this light source */
        LightSourceType mLightSourceType;

        /** Color of the light source */
        Vec3f mLightColor = Vec3f(1.0f);

        /** Relative position point */
        Vec3f mLocalPosition = Vec3f(0.0f);

        /** Absolute position point */
        Vec3f mWorldPosition = Vec3f(0.0f);

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

        /** Percentage for camera distance where light source shadows could be visible */
        float32 mShadowActionFactor = 1.0f;

        /** Shadow map buffer size */
        uint32 mShadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

        /** Pointer to relevant shadow map buffer */
        class IDepthBuffer* mShadowMap = nullptr;

        /** Previous registered component in render system  */
        class LightComponent* mPrev = nullptr;

        /** Next registered component in render system  */
        class LightComponent* mNext = nullptr;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_LIGHTCOMPONENT_H
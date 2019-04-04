//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_LIGHTSOURCECOMPONENT_H
#define BERSERK_LIGHTSOURCECOMPONENT_H

#include <Base/RenderBase.h>
#include <Platform/IDepthBuffer.h>
#include <Components/SceneComponent.h>

namespace Berserk::EntitySystem
{
    using namespace Resources;

    class ENGINE_API LightSourceComponent : public RenderBase, public SceneComponent
    {
    public:

        /** Default object setup via initializer */
        explicit LightSourceComponent(const IObjectInitializer& objectInitializer)
                : SceneComponent(objectInitializer),
                  mHasLensOpticsEffect(FIELD_OFF),
                  mHasLightShaftsEffect(FIELD_OFF),
                  mHasVolumetricLightEffect(FIELD_OFF)
        {

        }

        /** Do actually nothing */
        ~LightSourceComponent() override = default;

    public:


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

        static const uint32 DEFAULT_SHADOW_MAP_SIZE = 512;

        static const float32 DEFAULT_SHADOW_MAP_BIAS;

        /** Color of the light source */
        Vec4f mLightColor = Vec4f(1.0f,1.0f,1.0f,1.0f);

        /** Its position in local space */
        Vec4f mLightPosition;

        /** Shows if that light casts shadow */
        uint8 mCastShadows : 1;

        /** If can apply to this lens optics effect */
        uint8 mHasLensOpticsEffect : 1;

        /** If can apply to this Light Shafts effect (God's rays) */
        uint8 mHasLightShaftsEffect : 1;

        /** If can apply volumetric light (ray tracing) effect */
        uint8 mHasVolumetricLightEffect : 1;

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

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_LIGHTSOURCECOMPONENT_H
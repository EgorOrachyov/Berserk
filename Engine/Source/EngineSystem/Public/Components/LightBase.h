//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_LIGHTBASE_H
#define BERSERK_LIGHTBASE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    /** Engine build in light types */
    enum ELightSourceType
    {
        LST_SpotLight,
        LST_PointLight,
        LST_DirectionalLight,
    };

    /** Base class for main and render thread light source types */
    class ENGINE_API LightBase
    {
    public:

        /** @return Type of this light source */
        ELightSourceType getLightSourceType() const { return mLightSourceType; }

        /** @return True if that light casts shadow */
        bool castShadows() const                { return mCastShadows; }

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

        /** @return Percentage for camera distance where light source shadows could be visible  */
        float32 getShadowActionFactor() const   { return mShadowActionFactor; }

        /** @return Shadow map buffer size */
        uint32 getShadowMapSize() const         { return mShadowMapSize; }

    private:

        /** Default shadow map size in pixels */
        static const uint32 DEFAULT_SHADOW_MAP_SIZE = 512;

        /** Default bias to avoid depth fighting */
        static const float32 DEFAULT_SHADOW_MAP_BIAS;

        /** Type of this light source */
        ELightSourceType mLightSourceType;

        /** Color of the light source */
        Vec3f mLightColor = Vec3f(1.0f);

        /** Relative position point */
        Vec3f mLocalPosition = Vec3f(0.0f);

        /** Absolute position point */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** Shows if that light casts shadow */
        bool mCastShadows;

        /** Distance of action */
        float32 mLightMaxDistance = 1.0f;

        /** Control depth map test mistake */
        float32 mShadowMapBias = DEFAULT_SHADOW_MAP_BIAS;

        /** Percentage for camera distance where light source shadows could be visible */
        float32 mShadowActionFactor = 1.0f;

        /** Shadow map buffer size */
        uint32 mShadowMapSize = DEFAULT_SHADOW_MAP_SIZE;

    };

} // namespace Berserk

#endif //BERSERK_LIGHTBASE_H
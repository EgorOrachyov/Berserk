//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_LIGHTBASE_H
#define BERSERK_LIGHTBASE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Misc/Bits.h>
#include <Math/MathInclude.h>

namespace Berserk
{

    /** Engine build in light types */
    enum ELightSourceType : uint8
    {
        LST_SpotLight,
        LST_PointLight,
        LST_DirectionalLight,
    };

    /** Flags to define which part of the light was changed */
    enum ELightDirtyFlag : uint8
    {
        LDF_Transform = SHIFT(0),
        LDF_Everything = SHIFT(1)
    };

    /** Base class for main and render thread light source types */
    class ENGINE_API LightBase
    {
    public:

        /** Set this light cast shadows or not [cause main and render thread sync] */
        void setCastShadows(bool castShadows)
        {
            mCastShadows = castShadows;
            light_markDirty();
        }

        /** Set this light active (affects on the scene) [cause main and render thread sync] */
        void setIsActive(bool active)
        {
            mIsActive = active;
            light_markDirty();
        }

        /** Set light color [cause main and render thread sync] */
        void setLightColor(const Vec3f& color)
        {
            mLightColor = color;
            light_markDirty();
        }

        /** Set world position [cause main and render thread sync] */
        void setWorldPosition(const Vec3f& position)
        {
            mWorldPosition = position;
            light_markDirty(ELightDirtyFlag::LDF_Transform);
        }

        /** Set world rotation [cause main and render thread sync] */
        void setWorldRotation(const Quatf& rotation)
        {
            mWorldRotation = rotation;
            light_markDirty(ELightDirtyFlag::LDF_Transform);
        }

        /** Set max distance of affecting on objects[cause main and render thread sync] */
        void setMaxLightDistance(float32 distance)
        {
            mLightMaxDistance = distance;
            light_markDirty();
        }

        /** Set shadow map bias for proper sampling [cause main and render thread sync] */
        void setShadowMapBias(float32 bias)
        {
            mShadowMapBias = bias;
            light_markDirty();
        }

        /** @return Type of this light source */
        ELightSourceType getLightSourceType() const { return mLightSourceType; }

        /** @return True if that light casts shadow */
        bool castShadows() const { return mCastShadows; }

        /** @return True if this light is active on the scene */
        bool isActive() const { return mIsActive; }

        /** @return Color of this source */
        const Vec3f& getLightColor() const { return mLightColor; }

        /** @return World space position */
        const Vec3f& getWorldPosition() const { return mWorldPosition; }

        /** @return World space rotation */
        const Quatf& getWorldRotation() const { return mWorldRotation; }

        /** @return Distance of action */
        float32 getMaxLightDistance() const { return mLightMaxDistance; }

        /** @return Depth map test bias */
        float32 getShadowMapBias() const { return mShadowMapBias; }

    private:

        /**
         * Marks the light source data dirty and cause the following
         * main and rendering thread synchronisation
         */
        virtual void light_markDirty(uint32 flags = ELightDirtyFlag::LDF_Everything) { };

    private:

        /** Default shadow map size in pixels */
        static const uint32 DEFAULT_SHADOW_MAP_SIZE = 512;

        /** Default bias to avoid depth fighting */
        static const float32 DEFAULT_SHADOW_MAP_BIAS;

        /** Type of this light source */
        ELightSourceType mLightSourceType;

        /** Color of the light source */
        Vec3f mLightColor = Vec3f(1.0f);

        /** World space position */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** World space rotation */
        Quatf mWorldRotation = Quatf();

        /** Shows if that light casts shadow */
        bool mCastShadows;

        /** Whether this light is active in the scene */
        bool mIsActive;

        /** Distance of action */
        float32 mLightMaxDistance = 1.0f;

        /** Control depth map test mistake */
        float32 mShadowMapBias = DEFAULT_SHADOW_MAP_BIAS;

    };

} // namespace Berserk

#endif //BERSERK_LIGHTBASE_H
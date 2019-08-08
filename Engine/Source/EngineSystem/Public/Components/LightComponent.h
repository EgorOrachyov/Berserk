//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_LIGHTCOMPONENT_H
#define BERSERK_LIGHTCOMPONENT_H

#include <Math/MathInclude.h>
#include <Components/Component.h>

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
        LDF_Everything = SHIFT(1),
        LDF_Active = SHIFT(2),
        LDF_Settings = SHIFT(3)
    };

    /** Base class for main and render thread light source types */
    class ENGINE_API LightComponent : public Component
    {
    public:

        REFLECTABLE_OBJECT(LightComponent);

    public:

#if DEBUG
        LightComponent() : Component() {}
#endif

        ~LightComponent() override = default;

        /** Set this light cast shadows or not [cause main and render thread sync] */
        void setCastShadows(bool castShadows)
        {
            mCastShadows = castShadows;
            lightComponent_markDirty();
        }

        /** Set this light active (affects on the scene) [cause main and render thread sync] */
        void setIsActive(bool active)
        {
            bool wasActive = mIsActive;

            mIsActive = active;
            if (wasActive != active) lightComponent_markDirty(ELightDirtyFlag::LDF_Active);
        }

        /** Set light color [cause main and render thread sync] */
        void setLightColor(const Vec3f& color)
        {
            mLightColor = color;
            lightComponent_markDirty();
        }

        /** Set world position [cause main and render thread sync] */
        void setWorldPosition(const Vec3f& position)
        {
            mWorldPosition = position;
            lightComponent_markDirty(ELightDirtyFlag::LDF_Transform);
        }

        /** Set world rotation [cause main and render thread sync] */
        void setWorldRotation(const Quatf& rotation)
        {
            mWorldRotation = rotation;
            lightComponent_markDirty(ELightDirtyFlag::LDF_Transform);
        }

        /** Set max distance of affecting on objects[cause main and render thread sync] */
        void setMaxLightDistance(float32 distance)
        {
            mLightMaxDistance = distance;
            lightComponent_markDirty();
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

    protected:

        /**
         * Marks the light source data dirty and cause the following
         * main and rendering thread synchronisation
         */
        virtual void lightComponent_markDirty(uint32 flags = ELightDirtyFlag::LDF_Everything) { };

    protected:

        /** Type of this light source */
        ELightSourceType mLightSourceType;

        /** Color of the light source */
        Vec3f mLightColor = Vec3f(1.0f);

        /** World space position */
        Vec3f mWorldPosition = Vec3f(0.0f);

        /** World space rotation */
        Quatf mWorldRotation = Quatf();

        /** Shows if that light casts shadow */
        bool mCastShadows = true;

        /** Whether this light is active in the scene */
        bool mIsActive = true;

        /** Distance of action */
        float32 mLightMaxDistance = 1.0f;

    };

} // namespace Berserk

#endif //BERSERK_LIGHTCOMPONENT_H
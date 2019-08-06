//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_DIRECTIONALLIGHTCOMPONENT_H
#define BERSERK_DIRECTIONALLIGHTCOMPONENT_H

#include <Components/LightBase.h>
#include <Components/Component.h>
#include <Engine/EngineCommon.h>

namespace Berserk
{

    /**
     * Light source component with parallel light rays. Could be used for emulating sun light.
     * Has only on shadow map buffer in rendering. Has adaptive view space view volume
     * calculation relatively to scene camera position, what allows to optimize shadow map generation.
     */
    class DirectionalLightComponent final : public Component, public LightBase
    {
    public:

        REFLECTABLE_OBJECT(DirectionalLightComponent);

    public:

        /** Set direction of the light */
        void setWorldDirection(const Vec3f& direction)
        {
            mWorldDirection = direction;
            light_markDirty(ELightDirtyFlag::LDF_Settings);
        }

        /** Set light up vector of the light */
        void setWorldUp(const Vec3f& up)
        {
            mWorldUp = up;
            light_markDirty(ELightDirtyFlag::LDF_Settings);
        }

        /** @return Wolrd up vector [default Z axis] */
        const Vec3f& getWorldDirection() const { return mWorldDirection; }

        /** @return World up vector [default Y axis] */
        const Vec3f& getWorldUp() const { return mWorldUp; }

    private:

        /** Absolute direction vector (unit length) */
        Vec3f mWorldDirection = Vec3f::axisZ;

        /** Absolute Up basis vector  (unit length) */
        Vec3f mWorldUp = Vec3f::axisY;

    };

} // namespace Berserk

#endif //BERSERK_DIRECTIONALLIGHTCOMPONENT_H
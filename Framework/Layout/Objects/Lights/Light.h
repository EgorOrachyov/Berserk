//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_LIGHT_H
#define BERSERKENGINE_LIGHT_H

#include "Objects/Actor.h"
#include "Components/ShadowCasterComponent.h"

namespace Berserk
{

    class Light : public Actor
    {
    public:

        Light(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~Light();

        void toggle();

        virtual void setLightIntensity(const Vector3f &intensity) = 0;
        void setToggleable(bool setIn = true);
        void setCastShadows(bool setIn = false);
        void setCastLightShafts(bool setIn = false);

        virtual const Vector3f& getLightIntensity() const = 0;
        bool isToggleable() const;
        bool castShadows() const;
        bool castLightShafts() const;
    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHT_H
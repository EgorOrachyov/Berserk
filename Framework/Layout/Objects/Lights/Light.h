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

        void setLightIntensity(const Vector3f &intensity);
        void setToggleable(bool setIn = true);
        void setCastShadows(bool setIn = false);

        Vector3f getLightIntensity() const;
        bool isToggleable() const;
        bool isCastShadows() const;

        ShadowCasterComponent* getShadowCaster();

    protected:

        Vector3f mLightIntensity;
        ShadowCasterComponent mShadowComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHT_H
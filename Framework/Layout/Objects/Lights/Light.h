//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_LIGHT_H
#define BERSERKENGINE_LIGHT_H

#include "Objects/Actor.h"

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

    protected:

        INT8 mCastShadows   : 1;
        INT8 mIsToggleable  : 1;

        Vector3f mLightIntensity;

    };

} // namespace Berserk

#endif //BERSERKENGINE_LIGHT_H
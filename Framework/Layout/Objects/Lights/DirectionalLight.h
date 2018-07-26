//
// Created by Egor Orachyov on 26.07.2018.
//

#ifndef BERSERKENGINE_DIRECTIONALLIGHT_H
#define BERSERKENGINE_DIRECTIONALLIGHT_H

#include "Light.h"

namespace Berserk
{

    class DirectionalLight : public Light
    {
    public:

        DirectionalLight(const CStaticString &name, FLOAT32 lifeTime = 0);
        virtual ~DirectionalLight() = default;

        void setDirection(const Vector3f &direction);
        Vector3f getDirection() const;

    private:

        Vector3f mDirection;

    };

} // namespace Berserk

#endif //BERSERKENGINE_DIRECTIONALLIGHT_H
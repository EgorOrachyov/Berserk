//
// Created by Egor Orachyov on 26.08.2018.
//

#ifndef BERSERKENGINE_GLOBALLIGHT_H
#define BERSERKENGINE_GLOBALLIGHT_H

#include "DirectionalLight.h"

namespace Berserk
{

    class ENGINE_EXPORT GlobalLight : public DirectionalLight
    {
    public:

        GlobalLight(const CStaticString& name, FLOAT32 lifeTime = 0);
        virtual ~GlobalLight() = default;

        void setSphereRadius(FLOAT32 radius);
        void setSphereColor(const Vector3f& color);
        void setBackColor(const Vector3f& color);

        FLOAT32 getSphereRadius() const;
        const Vector3f& getSphereColor() const;
        const Vector3f& getBackColor() const;

    protected:

        void process(FLOAT64 delta, const Matrix4x4f &rootTransformation) override;

        void processGlobalLight();

    protected:

        FLOAT32     mSphereRadius;
        Vector3f    mSphereColor;
        Vector3f    mBackColor;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLOBALLIGHT_H
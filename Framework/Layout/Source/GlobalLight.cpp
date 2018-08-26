//
// Created by Egor Orachyov on 26.08.2018.
//

#include "Objects/Lights/GlobalLight.h"

namespace Berserk
{

    GlobalLight::GlobalLight(const CStaticString &name, FLOAT32 lifeTime) : DirectionalLight(name, lifeTime)
    {
        setSphereRadius(1.0);
        setSphereColor(Vector3f(0.5));
        setBackColor(Vector3f(0.1));
    }

    void GlobalLight::setSphereRadius(FLOAT32 radius)
    {
        if (mIsEditable)
        {
            mSphereRadius = radius;
        }
    }

    void GlobalLight::setSphereColor(const Vector3f &color)
    {
        if (mIsEditable)
        {
            mSphereColor = color;
        }
    }

    void GlobalLight::setBackColor(const Vector3f &color)
    {
        if (mIsEditable)
        {
            mBackColor = color;
        }
    }

    FLOAT32 GlobalLight::getSphereRadius() const
    {
        return mSphereRadius;
    }

    const Vector3f& GlobalLight::getSphereColor() const
    {
        return mSphereColor;
    }

    const Vector3f& GlobalLight::getBackColor() const
    {
        return mBackColor;
    }

    void GlobalLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        if (mIsActive)
        {
            processActor(delta, rootTransformation);
            processDirectionalLight(); // todo: add global light process
        }
    }

    void GlobalLight::processGlobalLight()
    {

    }

} // namespace Berserk
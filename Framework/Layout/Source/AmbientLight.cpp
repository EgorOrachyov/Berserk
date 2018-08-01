//
// Created by Egor Orachyov on 31.07.2018.
//

#include "Objects/Lights/AmbientLight.h"
#include "Managers/SceneManager.h"

namespace Berserk
{

    AmbientLight::AmbientLight(const CStaticString &name, FLOAT32 lifeTime) : Light(name, lifeTime)
    {

    }

    AmbientLight::~AmbientLight()
    {

    }

    AmbientLightComponent& AmbientLight::getAmbientComponent()
    {
        return mAmbientComponent;
    }

    void AmbientLight::process(FLOAT64 delta, const Matrix4x4f &rootTransformation)
    {
        Light::process(delta, rootTransformation);

        if (mIsActive)
        {
            mAmbientComponent.mLightIntensity = mLightIntensity;
            gSceneManager->getRenderManager().queueLight(&mAmbientComponent);
        }
    }

} // namespace Berserk
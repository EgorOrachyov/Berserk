//
// Created by Egor Orachyov on 27.07.2018.
//

#include "Objects/Materials/BaseMaterial.h"
#include "Managers/SceneManager.h"

namespace Berserk
{

    BaseMaterial::BaseMaterial(const CStaticString &name) : Material(name)
    {
        mMaterialComponent.mAmbient = Vector3f(0,0,0);
        mMaterialComponent.mDiffuse = Vector3f(0,0,0);
        mMaterialComponent.mSpecular = Vector3f(0,0,0);
        mMaterialComponent.mShininess = 1;
    }

    void BaseMaterial::setAmbientColor(const Vector3f &color)
    {
        mMaterialComponent.mAmbient = color;
    }

    void BaseMaterial::setDiffuseColor(const Vector3f &color)
    {
        mMaterialComponent.mDiffuse = color;
    }

    void BaseMaterial::setSpecularColor(const Vector3f &color)
    {
        mMaterialComponent.mSpecular = color;
    }

    void BaseMaterial::setShininess(FLOAT32 shininess)
    {
        mMaterialComponent.mShininess = shininess;
    }

    BaseMaterialComponent &BaseMaterial::getBaseMaterialComponent()
    {
        return mMaterialComponent;
    }

    void BaseMaterial::apply()
    {
        gSceneManager->getRenderManager().queueMaterial(&mMaterialComponent);
    }
    
} // namespace Berserk
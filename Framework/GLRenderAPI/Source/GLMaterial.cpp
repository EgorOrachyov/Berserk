//
// Created by Egor Orachyov on 08.08.2018.
//

#include "Render/RenderSystem.h"
#include "Render/GLMaterial.h"
#include "Managers/GLTextureManager.h"
#include "Essential/Assert.h"
#include "Logging/LogMessages.h"


namespace Berserk
{

    GLMaterial::GLMaterial()
    {
        mMask = 0;
        mReferenceCount = 0;

        mAmbientColor = Vector3f(0,0,0);
        mDiffuseColor = Vector3f(0,0,0);
        mSpecularColor = Vector3f(0,0,0);
        mShininess = 1.0;

        mNormalMap = nullptr;
        mDiffuseMap = nullptr;
        mSpecularMap = nullptr;
        mCubeMap = nullptr;
    }
    
    void GLMaterial::destroy()
    {
        PUSH("Destroy material with name %s", mName.getChars());

        auto *manager = dynamic_cast<GLTextureManager*>(gRenderSystem->getTextureManagerPtr());

        if (mNormalMap)
        {
            manager->deleteTexture(mNormalMap);
        }
        if (mSpecularMap)
        {
            manager->deleteTexture(mSpecularMap);
        }
        if (mDiffuseMap)
        {
            manager->deleteTexture(mDiffuseMap);
        }
        if (mCubeMap)
        {
            manager->deleteTexture(mCubeMap);
        }
    }

    UINT32 GLMaterial::getMemoryUsage() const
    {
        return sizeof(GLMaterial);
    }

    void   GLMaterial::addReference()
    {
        mReferenceCount += 1;
    }

    void   GLMaterial::release()
    {
        if (mReferenceCount > 0) mReferenceCount -= 1;
        if (mReferenceCount == 0) destroy();
    }

    UINT32 GLMaterial::getReferences() const
    {
        return mReferenceCount;
    }

    void  GLMaterial::setName(const CStaticString& name)
    {
        mName = name;
    }

    const CStaticString& GLMaterial::getName() const
    {
        return CStaticString(mName.getChars());
    }

    void   GLMaterial::setType(UINT32 mask)
    {
        mMask = mask;
    }

    UINT32 GLMaterial::getType() const
    {
        return mMask;
    }

    void GLMaterial::setAmbientComponent(const Vector3f& component)
    {
        mAmbientColor = component;
    }

    void GLMaterial::setDiffuseComponent(const Vector3f& component)
    {
        mDiffuseColor = component;
    }

    void GLMaterial::setSpecularComponent(const Vector3f& component)
    {
        mSpecularColor = component;
    }

    void GLMaterial::setShininess(FLOAT32 shininess)
    {
        mShininess = shininess;
    }

    const Vector3f& GLMaterial::getAmbientComponent() const
    {
        return mAmbientColor;
    }

    const Vector3f& GLMaterial::getDiffuseComponent() const
    {
        return mDiffuseColor;
    }

    const Vector3f& GLMaterial::getSpecularComponent() const
    {
        return mSpecularColor;
    }

    FLOAT32 GLMaterial::getShininess() const
    {
        return mShininess;
    }

    void GLMaterial::setNormalMap(Texture* map)
    {
        ASSERT(map, "GLMaterial: NULL Normal Map for material %s", mName.getChars());
        mNormalMap = dynamic_cast<GLTexture*>(map);
    }

    void GLMaterial::setDiffuseMap(Texture* map)
    {
        ASSERT(map, "GLMaterial: NULL Diffuse Map for material %s", mName.getChars());
        mDiffuseMap = dynamic_cast<GLTexture*>(map);
    }

    void GLMaterial::setSpecularMap(Texture* map)
    {
        ASSERT(map, "GLMaterial: NULL Specular Map for material %s", mName.getChars());
        mSpecularMap = dynamic_cast<GLTexture*>(map);
    }

    void GLMaterial::setCubeMap(Texture* map)
    {
        ASSERT(map, "GLMaterial: NULL Cube Map for material %s", mName.getChars());
        mCubeMap = dynamic_cast<GLTexture*>(map);
    }

    Texture* GLMaterial::getNormalMap() const
    {
        return mNormalMap;
    }

    Texture* GLMaterial::getDiffuseMap() const
    {
        return mDiffuseMap;
    }

    Texture* GLMaterial::getSpecularMap() const
    {
        return mSpecularMap;
    }

    Texture* GLMaterial::getCubeMap() const
    {
        return mCubeMap;
    }

    bool GLMaterial::isLoaded() const
    {
        bool isLoaded = true;

        isLoaded &= (!(mMask & MaterialType::MT_NORMAL_MAPPED) || (mNormalMap != nullptr));
        isLoaded &= (!(mMask & MaterialType::MT_SPECULAR_MAPPED) || (mSpecularMap != nullptr));
        isLoaded &= (!(mMask & MaterialType::MT_DIFFUSE_MAPPED) || (mDiffuseMap != nullptr));
        isLoaded &= (!(mMask & MaterialType::MT_CUBE_MAPPED) || (mCubeMap != nullptr));

        return isLoaded;
    }

    GLMaterial& GLMaterial::operator = (const GLMaterial &material)
    {
        mMask = material.mMask;
        mReferenceCount = material.mReferenceCount;

        mAmbientColor = material.mAmbientColor;
        mDiffuseColor = material.mDiffuseColor;
        mSpecularColor = material.mSpecularColor;
        mShininess = material.mShininess;

        mNormalMap = material.mNormalMap;
        mDiffuseMap = material.mDiffuseMap;
        mSpecularMap = material.mSpecularMap;
        mCubeMap = material.mCubeMap;

        mName = material.mName;

        return *this;
    }

    const bool GLMaterial::operator == (const GLMaterial &material) const
    {
        return mName == material.mName;
    }

} // namespace Berserk
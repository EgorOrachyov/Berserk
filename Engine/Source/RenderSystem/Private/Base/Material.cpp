//
// Created by Egor Orachyov on 24.03.2019.
//

#include "Managers/ITextureManager.h"
#include "Base/Material.h"

namespace Berserk::Resources
{

    Material::Material(const char *name, ITextureManager *manager)
            : mResourceName(name),
              mTextureManager(manager),
              mMaterialType(0x0),
              mMaterialLayers(0x0),
              mAlbedoMap(nullptr),
              mNormalMap(nullptr),
              mMetallicMap(nullptr),
              mRoughnessMap(nullptr),
              mAmbientMap(nullptr),
              mDisplacementMap(nullptr),
              mDefaultColor(1.0f),
              mEmissiveColor(0.0f),
              mWireFrameColor(1.0),
              mReferenceCount(0)
    {
        FAIL(name, "Null pointer string name");
        FAIL(manager, "Null pointer texture manager");
    }

    Material::~Material()
    {
        /* something */
    }

    void Material::addReference()
    {
        mReferenceCount += 1;
    }


    void Material::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0)
        {
            for (uint32 i = 0; i < eML_SUPPORTED_LAYERS; i++)
            {
                if (mTextures[i])
                {
                    mTextureManager->deleteTexture(mTextures[i]);
                    mTextures[i] = nullptr;
                }
            }
        }
    }

    uint32 Material::getReferenceCount()
    {
        return mReferenceCount;
    }

    const char* Material::getName()
    {
        return mResourceName.get();
    }

    void Material::setMaterialType(uint32 type)
    {
        mMaterialType = type;
    }

    void Material::setMaterialLayer(MaterialLayer layer, bool active)
    {
        uint32 current = eML_NOT_MATERIAL_LAYER;

        for (uint32 i = 0; i < eML_SUPPORTED_LAYERS; i++)
        {
            current = current << 1;
            if ((current == layer) && (mTextures[i] != nullptr))
            {
                mMaterialLayers |= current;
                return;
            }
        }
    }

    void Material::setMaterialLayers(uint32 layersMask)
    {
        uint32 current = eML_NOT_MATERIAL_LAYER;
        mMaterialLayers = 0;

        for (uint32 i = 0; i < eML_SUPPORTED_LAYERS; i++)
        {
            current = current << 1;
            if ((layersMask & current != 0) && (mTextures[i] != nullptr))
            {
                mMaterialLayers |= current;
            }
        }
    }

    uint32 Material::getMaterialType()
    {
        return mMaterialType;
    }

    uint32 Material::getMaterialLayers()
    {
        return mMaterialLayers;
    }

    uint32 Material::getNumAttachedLayers()
    {
        uint32 layers = 0;

        for (uint32 i = 0; i < eML_SUPPORTED_LAYERS; i++)
        {
            layers += (mTextures[i] != nullptr);
        }

        return layers;
    }

    uint32 Material::getGPUMemoryUsage()
    {
        uint32 gpuMemUsage = 0;

        for (uint32 i = 0; i < eML_SUPPORTED_LAYERS; i++)
        {
            if (mTextures[i]) gpuMemUsage += mTextures[i]->getGPUMemoryUsage();
        }

        return gpuMemUsage;
    }

    const Vec4f& Material::getDefaultColor() const
    {
        return mDefaultColor;
    }

    const Vec4f& Material::getEmissiveColor() const
    {
        return mEmissiveColor;
    }

    const Vec4f& Material::getWireFrameColor() const
    {
        return mWireFrameColor;
    }

    const ITexture* Material::getMaterialLayer(MaterialLayer layer) const
    {
        switch (layer)
        {
            case eML_ALBEDO_MAP:
                return mAlbedoMap;

            case eML_NORMAL_MAP:
                return mNormalMap;

            case eML_METALLIC_MAP:
                return mMetallicMap;

            case eML_ROUGHNESS_MAP:
                return mRoughnessMap;

            case eML_AMBIENT_MAP:
                return mAmbientMap;

            case eML_DISPLACEMENT_MAP:
                return mDisplacementMap;

            default:
                WARNING("Unknown material layer type");
                return nullptr;
        }
    }

} // namespace Berserk::Resources
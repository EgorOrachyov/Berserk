//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include "Foundation/IMaterial.h"
#include "Platform/ITexture.h"
#include "Object/NewDelete.h"
#include "Strings/String.h"
#include "Managers/ITextureManager.h"

namespace Berserk::Resources
{

    /**
     * Main engine material class
     */
    class ENGINE_API Material : public IMaterial
    {
    public:

        GENARATE_NEW_DELETE(Material);

        /** Initialize material with chosen name */
        explicit Material(const char *name, ITextureManager *manager);

        /** Release resource */
        ~Material() override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char *getName() override;

    public:

        /** @copydoc IMaterial::setMaterialType() */
        void setMaterialType(uint32 type) override;

        /** @copydoc IMaterial::setMaterialLayer() */
        void setMaterialLayer(MaterialLayer layer, bool active) override;

        /** @copydoc IMaterial::setMaterialLayers() */
        void setMaterialLayers(uint32 layersMask) override;

        /** @copydoc IMaterial::getMaterialDebugInfo() */
        void getMaterialDebugInfo(char* buffer) override;

        /** @copydoc IMaterial::getMaterialType() */
        uint32 getMaterialType() override;

        /** @copydoc IMaterial::getMaterialLayers() */
        uint32 getMaterialLayers() override;

        /** @copydoc IMaterial::getNumAttachedLayers() */
        uint32 getNumAttachedLayers() override;

        /** @copydoc IMaterial::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

        /** @copydoc IMaterial::getDefaultColor() */
        const Vec4f& getDefaultColor() const override;

        /** @copydoc IMaterial::getEmissiveColor() */
        const Vec4f& getEmissiveColor() const override;

        /** @copydoc IMaterial::getWireFrameColor() */
        const Vec4f& getWireFrameColor() const override;

        /** @copydoc IMaterial::getMaterialLayer() */
        ITexture* getMaterialLayer(MaterialLayer layer) const override;

    public:

        friend class MaterialManagerHelper;

        uint32 mMaterialType;
        uint32 mMaterialLayers;
        uint32 mReferenceCount;

        Vec4f mDefaultColor;
        Vec4f mEmissiveColor;
        Vec4f mWireFrameColor;

        union
        {
            struct
            {
                ITexture* mAlbedoMap;
                ITexture* mNormalMap;
                ITexture* mMetallicMap;
                ITexture* mRoughnessMap;
                ITexture* mAmbientMap;
                ITexture* mDisplacementMap;
            };

            ITexture* mTextures[eML_SUPPORTED_LAYERS];
        };


        CString mResourceName;

        ITextureManager* mTextureManager;

    };

} // namespace Berserk::Resources

#endif //BERSERK_MATERIAL_H
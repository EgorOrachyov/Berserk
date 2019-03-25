//
// Created by Egor Orachyov on 24.03.2019.
//

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include "Base/IMaterial.h"
#include "Platform/ITexture.h"

namespace Berserk::Resources
{

    class ENGINE_API Material : public IMaterial
    {
    public:

        Material(const char* name);

        ~Material() override;

        void setMaterialType(uint32 type) override;

        void setMaterialLayer(MaterialLayer layer, bool active) override;

        void setMaterialLayers(uint32 layersMask) override;

        uint32 getMaterialType() override;

        uint32 getMaterialLayers() override;

        uint32 getNumAttachedLayers() override;

        uint32 getGPUMemoryUsage() override;

        const Vec4f& getColor() const override;

        const Vec4f& getEmissiveColor() const override;

        const Vec4f& getWireFrameColor() const override;

        const ITexture* getMaterialLayer(MaterialLayer layer) const override;

    public:

        uint32 mMaterialType;
        uint32 mMaterialLayers;

        Vec4f mColor;
        Vec4f mEmissiveColor;
        Vec4f mWireFrameColor;

        ITexture* mAlbedoMap;
        ITexture* mNormalMap;
        ITexture* mMetallicMap;
        ITexture* mRoughnessMap;
        ITexture* mAmbientMap;
        ITexture* mDisplacementMap;

    };

} // namespace Berserk::Resources

#endif //BERSERK_MATERIAL_H
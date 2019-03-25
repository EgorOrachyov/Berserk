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

        virtual void setMaterialType(uint32 type) = 0;

        virtual void setMaterialLayer(MaterialLayer layer, bool active) = 0;

        virtual void setMaterialLayers(uint32 layersMask) = 0;

        virtual uint32 getMaterialType() = 0;

        virtual uint32 getMaterialLayers() = 0;

        virtual uint32 getNumAttachedLayers() = 0;

        virtual uint32 getGPUMemoryUsage() = 0;

        virtual const Vec4f& getColorComponent() const = 0;

        virtual const ITexture* getMaterialLayer(MaterialLayer layer) const = 0;

    public:

        uint32 mMaterialType;
        uint32 mMaterialLayers;

        Vec4f mColorComponent;
        Vec4f mEmissiveComponent;
        Vec4f mWireFrameComponent;

        ITexture* mAlbedoMap;
        ITexture* mNormalMap;
        ITexture* mMetallicMap;
        ITexture* mRoughnessMap;
        ITexture* mAmbientMap;
        ITexture* mDisplacementMap;

    };

} // namespace Berserk::Resources

#endif //BERSERK_MATERIAL_H
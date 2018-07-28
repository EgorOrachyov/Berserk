//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_BASEMATERIAL_H
#define BERSERKENGINE_BASEMATERIAL_H

#include "Objects/Resource.h"
#include "Components/BaseMaterialComponent.h"

namespace Berserk
{

    class BaseMaterial : public Resource
    {
    public:

        BaseMaterial(const CStaticString &name);
        virtual ~BaseMaterial() = default;

        void load(const CString &fullFileName) override;
        void load(const CString &fileName, const CString &path) override;
        void use() override;

        bool isLoaded() const override;

        void setAmbientColor(const Vector3f &color);
        void setDiffuseColor(const Vector3f &color);
        void setSpecularColor(const Vector3f &color);
        void setShininess(FLOAT32 shininess);

        BaseMaterialComponent &getBaseMaterialComponent();

    private:

        BaseMaterialComponent mMaterialComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_BASEMATERIAL_H
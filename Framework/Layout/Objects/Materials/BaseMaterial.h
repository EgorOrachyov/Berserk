//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_BASEMATERIAL_H
#define BERSERKENGINE_BASEMATERIAL_H

#include "Material.h"
#include "Components/BaseMaterialComponent.h"

namespace Berserk
{

    class BaseMaterial : public Material
    {
    public:

        BaseMaterial(const CStaticString &name);
        virtual ~BaseMaterial() = default;

        void setAmbientColor(const Vector3f &color);
        void setDiffuseColor(const Vector3f &color);
        void setSpecularColor(const Vector3f &color);
        void setShininess(FLOAT32 shininess);

        BaseMaterialComponent &getBaseMaterialComponent();

    protected:

        void apply() override;

    private:

        BaseMaterialComponent mMaterialComponent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_BASEMATERIAL_H
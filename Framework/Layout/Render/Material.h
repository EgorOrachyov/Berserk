//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_MATERIAL_H
#define BERSERKENGINE_MATERIAL_H

#include "RenderResource.h"
#include "Math/Vector3f.h"
#include "Texture.h"

namespace Berserk
{
    enum MaterialType
    {
        MT_BASIC            = 0x0001,
        MT_NORMAL_MAPPED    = 0x0002,
        MT_SPECULAR_MAPPED  = 0x0004,
        MT_DIFFUSE_MAPPED   = 0x0008,
        MT_CUBE_MAPPED      = 0x0010,
        MT_NO_SHADOW        = 0x0020
    };

    class Material : public RenderResource
    {
    public:

        virtual ~Material() = default;

        virtual void destroy() = 0;
        virtual void clone(Material* newInstance) = 0;

        virtual void setType(UINT32 mask) = 0;
        virtual UINT32 getType() const = 0;

        virtual void setAmbientComponent(const Vector3f& component) = 0;
        virtual void setDiffuseComponent(const Vector3f& component) = 0;
        virtual void setSpecularComponent(const Vector3f& component) = 0;
        virtual void setShininess(FLOAT32 shininess) = 0;

        virtual const Vector3f& getAmbientComponent() const = 0;
        virtual const Vector3f& getDiffuseComponent() const = 0;
        virtual const Vector3f& getSpecularComponent() const = 0;
        virtual FLOAT32 getShininess() const = 0;

        virtual void setNormalMap(Texture* map) = 0;
        virtual void setDiffuseMap(Texture* map) = 0;
        virtual void setSpecularMap(Texture* map) = 0;
        virtual void setCubeMap(Texture* map) = 0;

        virtual Texture* getNormalMap() const = 0;
        virtual Texture* getDiffuseMap() const = 0;
        virtual Texture* getSpecularMap() const = 0;
        virtual Texture* getCubeMap() const = 0;

        virtual bool isLoaded() const = 0;
    };
}

#endif //BERSERKENGINE_MATERIAL_H
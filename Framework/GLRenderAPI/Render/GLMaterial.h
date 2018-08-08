//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_GLMATERIAL_H
#define BERSERKENGINE_GLMATERIAL_H

#include "Strings/CString.h"
#include "Render/Material.h"
#include "Texture/GLTexture.h"

namespace Berserk
{

    class GLMaterial : public Material
    {
    public:

        GLMaterial();
        virtual ~GLMaterial() = default;

        void destroy() override;
        void clone(Material* material) override;

        UINT32 getMemoryUsage() const override;

        void   addReference() override;
        void   release() override;
        UINT32 getReferences() const override;

        void  setName(const CStaticString& name) override;
        const CStaticString& getName() const override;

        void   setType(UINT32 mask) override;
        UINT32 getType() const override;

        void setAmbientComponent(const Vector3f& component) override;
        void setDiffuseComponent(const Vector3f& component) override;
        void setSpecularComponent(const Vector3f& component) override;
        void setShininess(FLOAT32 shininess) override;

        const Vector3f& getAmbientComponent() const override;
        const Vector3f& getDiffuseComponent() const override;
        const Vector3f& getSpecularComponent() const override;
        FLOAT32 getShininess() const override;

        void setNormalMap(Texture* map) override;
        void setDiffuseMap(Texture* map) override;
        void setSpecularMap(Texture* map) override;
        void setCubeMap(Texture* map) override;

        Texture* getNormalMap() const override;
        Texture* getDiffuseMap() const override;
        Texture* getSpecularMap() const override;
        Texture* getCubeMap() const override;

        bool isLoaded() const override;

        const bool operator == (const GLMaterial& material) const;

    private:

        UINT32 mMask;
        UINT32 mReferenceCount;

        Vector3f mAmbientColor;
        Vector3f mDiffuseColor;
        Vector3f mSpecularColor;
        FLOAT32  mShininess;

        GLTexture* mNormalMap;
        GLTexture* mDiffuseMap;
        GLTexture* mSpecularMap;
        GLTexture* mCubeMap;

        CStaticString mName;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLMATERIAL_H
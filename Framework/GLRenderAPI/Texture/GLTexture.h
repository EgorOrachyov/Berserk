//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLTEXTURE_H
#define BERSERKENGINE_GLTEXTURE_H

#include "Essential/Types.h"
#include "Essential/GLDataType.h"
#include "Strings/CString.h"

#include "Render/Texture.h"

#include "GLSampler.h"
#include "GLTextureFormat.h"
#include "GLTextureMipmaps.h"

namespace Berserk
{

    class GLTexture : public Texture
    {
    public:

        GLTexture();
        virtual ~GLTexture() = default;

        void destroy();
        UINT32 getMemoryUsage() const override;

        void   addReference() override;
        void   release() override;
        UINT32 getReferences() const override;

        void setName(const CStaticString& name) override;
        const CStaticString& getName() const override;

        void setType(TextureType type) override;
        TextureType getType() const override;

        void create(UINT32 width, UINT32 height);
        void create(UINT32 width, UINT32 height, GLInternalTextureFormat format);
        void create(UINT32 width, UINT32 height, GLInternalTextureFormat format,
                    GLImageFormat source, GLDataType actualType, void* data, GLMipmaps gen = GLMipmaps::GLM_USE);

        void use(UINT32 textureSlot) const override;

        void setSampler(GLSampler* sampler);
        GLSampler* getSampler() const;

        UINT32 getHandle() const;
        UINT32 getWidth() const override;
        UINT32 getHeight() const override;
        void   getSize(UINT32& width, UINT32& height) const override;

        bool isMipMapsGenerated() const override;
        bool isLoaded() const override;

        const bool operator == (const GLTexture& texture) const;

    private:

        UINT32 mWidth;
        UINT32 mHeight;
        UINT32 mHandle;
        UINT32 mReferenceCount;

        INT8 mGenMipMaps : 1;

        TextureType mType;
        GLInternalTextureFormat mFormat;

        CStaticString mName;
        GLSampler* mSampler;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTEXTURE_H
//
// Created by Egor Orachyov on 16.07.2018.
//

#ifndef BERSERKENGINE_GLFRAMEBUFFEROBJECT_H
#define BERSERKENGINE_GLFRAMEBUFFEROBJECT_H

#include "Texture/GLTexture.h"
#include "Texture/GLSampler.h"
#include "Essential/Types.h"
#include "Containers/LinkedList.h"

namespace Berserk
{

    class GLFrameBufferObject
    {
    private:

        struct DataLayout
        {
        public:

            DataLayout(UINT32 id, UINT16 attachment, UINT16 slot)
            {
                handle = id;
                shaderAttachment = attachment;
                textureSlot = slot;
            }

            UINT32 handle;
            UINT16 shaderAttachment;
            UINT16 textureSlot;
        };

    public:

        GLFrameBufferObject();
        ~GLFrameBufferObject();

        void init(UINT32 width, UINT32 height);
        void destroy();

        void addTexture(GLInternalTextureFormat format, GLWrapping wrapping, GLFiltering filtering, UINT16 shaderAttachment, UINT16 textureSlot);
        void addTexture(GLInternalTextureFormat format, UINT32 width, UINT32 height, GLWrapping wrapping, GLFiltering filtering, UINT16 shaderAttachment, UINT16 textureSlot);
        void addShadowMap(GLFiltering filtering);
        void addGeometryBuffer(GLInternalTextureFormat format, UINT16 shaderAttachment, UINT16 textureSlot);
        void addDepthBuffer();
        void addDepthStencilBuffer();

        void setShaderAttachments();
        void setShaderAttachmentsArray(UINT32 count, UINT32* data);

        void disable() const;
        void useAsFBO() const;
        void useAsUniformData();

        UINT32 getFBO() const;
        UINT32 getRBO() const;
        UINT32 getShadowMap() const;
        UINT32 getWidth() const;
        UINT32 getHeight() const;

    private:

        UINT32 mFBOHandle;
        UINT32 mRBOHandle;
        UINT32 mShadowMap;
        UINT32 mWidth;
        UINT32 mHeight;
        LinkedList<DataLayout> mLayouts;


    };

} // namespace Berserk

#endif //BERSERKENGINE_GLFRAMEBUFFEROBJECT_H
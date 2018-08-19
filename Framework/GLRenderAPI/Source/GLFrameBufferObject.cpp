//
// Created by Egor Orachyov on 16.07.2018.
//

#include "Buffers/GLFrameBufferObject.h"
#include "Essential/Assert.h"
#include "Logging/LogMessages.h""

namespace Berserk
{

    GLFrameBufferObject::GLFrameBufferObject()
    {
        mFBOHandle = 0;
        mRBOHandle = 0;
        mShadowMap = 0;
        mWidth = 0;
        mHeight = 0;

        mLayouts.init(4);
    }

    GLFrameBufferObject::~GLFrameBufferObject()
    {
        destroy();
    }

    void GLFrameBufferObject::init(UINT32 width, UINT32 height)
    {
        if (mFBOHandle)
        {
            WARNING("Frame Buffer Object has been initialized");
            return;
        }
        if (!(width || height))
        {
            WARNING("An attempt to create FBO with illegal size %u x %u", width, height);
            return;
        }

        mWidth = width;
        mHeight = height;

        glGenFramebuffers(1, &mFBOHandle);
    }

    void GLFrameBufferObject::destroy()
    {
        if (mFBOHandle)
        {
            glDeleteFramebuffers(1, &mFBOHandle);
        }
        if (mRBOHandle)
        {
            glDeleteRenderbuffers(1, &mRBOHandle);
        }
        if (mShadowMap)
        {
            glDeleteTextures(1, &mShadowMap);
        }

        mFBOHandle = 0;
        mRBOHandle = 0;
        mShadowMap = 0;
        mWidth = 0;
        mHeight = 0;

        mLayouts.iterate(true);
        while (mLayouts.iterate())
        {
            glDeleteTextures(1, &mLayouts.getCurrent().handle);
        }
        mLayouts.empty();
    }

    void GLFrameBufferObject::addTexture(GLInternalTextureFormat format, GLWrapping wrapping, GLFiltering filtering, UINT16 shaderAttachment, UINT16 textureSlot)
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        UINT32 texture = 0;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_INT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + shaderAttachment, GL_TEXTURE_2D, texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mLayouts.add(DataLayout(texture, shaderAttachment, textureSlot));
    }

    void GLFrameBufferObject::addTexture(GLInternalTextureFormat format, UINT32 width, UINT32 height,
                                         GLWrapping wrapping, GLFiltering filtering, UINT16 shaderAttachment,
                                         UINT16 textureSlot)
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        UINT32 texture = 0;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + shaderAttachment, GL_TEXTURE_2D, texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mLayouts.add(DataLayout(texture, shaderAttachment, textureSlot));
    }

    void GLFrameBufferObject::addShadowMap(GLFiltering filtering)
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }
        if (mShadowMap)
        {
            WARNING("Shadow map has been created");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        FLOAT32 borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glGenTextures(1, &mShadowMap);
        glBindTexture(GL_TEXTURE_2D, mShadowMap);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLFrameBufferObject::addGeometryBuffer(GLInternalTextureFormat format, UINT16 shaderAttachment, UINT16 textureSlot)
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        UINT32 texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RG8, GL_UNSIGNED_INT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + shaderAttachment, GL_TEXTURE_2D, texture, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        mLayouts.add(DataLayout(texture, shaderAttachment, textureSlot));
    }

    void GLFrameBufferObject::addDepthBuffer()
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }
        if (mRBOHandle)
        {
            WARNING("Render buffer object has been initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        glGenRenderbuffers(1, &mRBOHandle);
        glBindRenderbuffer(GL_RENDERBUFFER, mRBOHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mWidth, mHeight);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOHandle);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void GLFrameBufferObject::addDepthStencilBuffer()
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }
        if (mRBOHandle)
        {
            WARNING("Render buffer object has been initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);

        glGenRenderbuffers(1, &mRBOHandle);
        glBindRenderbuffer(GL_RENDERBUFFER, mRBOHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_RENDERBUFFER, mRBOHandle);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void GLFrameBufferObject::setShaderAttachments()
    {
        UINT32 data[BUFFER_SIZE_128];
        UINT32 count = 0;

        mLayouts.iterate(true);
        while (mLayouts.iterate())
        {
            data[count] = GL_COLOR_ATTACHMENT0 + mLayouts.getCurrent().shaderAttachment;
            count += 1;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glDrawBuffers(count, data);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBufferObject::setShaderAttachmentsArray(UINT32 count, UINT32* data)
    {
        if (!mFBOHandle)
        {
            WARNING("Frame Buffer Object is not initialized");
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
        glDrawBuffers(count, data);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBufferObject::useAsFBO() const
    {
        ASSERT(mFBOHandle, "Frame Buffer Object is not initialized");
        glBindFramebuffer(GL_FRAMEBUFFER, mFBOHandle);
    }

    void GLFrameBufferObject::disable() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBufferObject::useAsUniformData()
    {
        ASSERT(mFBOHandle, "Frame Buffer Object is not initialized");

        mLayouts.iterate(true);
        while (mLayouts.iterate())
        {
            glActiveTexture(GL_TEXTURE0 + mLayouts.getCurrent().textureSlot);
            glBindTexture(GL_TEXTURE_2D, mLayouts.getCurrent().handle);
        }
    }

    UINT32 GLFrameBufferObject::getFBO() const
    {
        return mFBOHandle;
    }

    UINT32 GLFrameBufferObject::getRBO() const
    {
        return mRBOHandle;
    }

    UINT32 GLFrameBufferObject::getShadowMap() const
    {
        return mShadowMap;
    }

    UINT32 GLFrameBufferObject::getWidth() const
    {
        return mWidth;
    }

    UINT32 GLFrameBufferObject::getHeight() const
    {
        return mHeight;
    }

} // namespace Berserk
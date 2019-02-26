//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Platform/GLFrameBuffer.h"
#include "Platform/GLInclude.h"
#include "Misc/Assert.h"

namespace Berserk
{

    void GLFrameBuffer::initialize(const char *name)
    {
        mWidth = 0;
        mHeigth = 0;
        mFrameBufferID = 0;
        mReferenceCount = 0;

        mResourceName = name;
    }

    void GLFrameBuffer::addReference()
    {
        mReferenceCount += 1;
    }

    void GLFrameBuffer::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0 && mFrameBufferID)
        {
            PUSH("GLFrameBuffer: delete [name: %s]", mResourceName.get());

            glDeleteFramebuffers(1, &mFrameBufferID);
            mFrameBufferID = 0;

            for (uint32 i = 0; i < mColorAttachments.getSize(); i++)
            {
                mColorAttachments[i].release();
            }

            mDepthBuffer.release();
        }
    }

    uint32 GLFrameBuffer::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLFrameBuffer::getMemoryUsage()
    {
        return sizeof(GLFrameBuffer);
    }

    const char* GLFrameBuffer::getName()
    {
        return mResourceName.get();
    }

    void GLFrameBuffer::createFrameBuffer(uint32 width, uint32 height)
    {
        if (mFrameBufferID)
        {
            FAIL(false, "An attempt to recreate frame buffer [name: %s]", mResourceName.get());
        }

        mWidth = width;
        mHeigth = height;

        glGenFramebuffers(1, &mFrameBufferID);
    }

    void GLFrameBuffer::attachColorBuffer(IRenderDriver::StorageFormat format)
    {
        if (!mFrameBufferID)
        {
            FAIL(false, "An attempt to attach buffer to not initialized frame buffer [name: %s]", mResourceName.get());
        }

    }

    void GLFrameBuffer::attachDepthBuffer()
    {
        if (!mFrameBufferID)
        {
            FAIL(false, "An attempt to attach buffer to not initialized frame buffer [name: %s]", mResourceName.get());
        }

        mDepthBuffer.initialize("DepthBuffer");
        mDepthBuffer.create(mWidth, mHeigth, IRenderDriver::DEPTH24);
        mDepthBuffer.setFiltering(IRenderDriver::FILTER_NEAREST, IRenderDriver::FILTER_NEAREST);
        mDepthBuffer.setWrapping(IRenderDriver::WRAP_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer.mTextureID, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::attachDepthStencilBuffer()
    {
        if (!mFrameBufferID)
        {
            FAIL(false, "An attempt to attach buffer to not initialized frame buffer [name: %s]", mResourceName.get());
        }

        mDepthBuffer.initialize("DepthBuffer");
        mDepthBuffer.create(mWidth, mHeigth, IRenderDriver::DEPTH24_STENCIL8);
        mDepthBuffer.setFiltering(IRenderDriver::FILTER_NEAREST, IRenderDriver::FILTER_NEAREST);
        mDepthBuffer.setWrapping(IRenderDriver::WRAP_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer.mTextureID, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::linkBuffers()
    {
        uint32 count = mColorAttachments.getSize();
        uint32 attachments[MAX_SUPPORTED_COLOR_BUFFERS];

        for (uint32 i = 0; i < count; i++)
        {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        };

        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        glDrawBuffers(count, attachments);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::bindFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
    }

    void GLFrameBuffer::bindColorBuffer(uint32 layer, uint32 textureSlot)
    {

    }

    void GLFrameBuffer::bindDepthBuffer(uint32 textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mDepthBuffer.mTextureID);
    }

    void GLFrameBuffer::bindDepthStencilBuffer(uint32 textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mDepthBuffer.mTextureID);
    }

    uint32 GLFrameBuffer::getGPUMemoryUsage()
    {

    }

    uint32 GLFrameBuffer::getMaxSupportedColorBuffers()
    {
        return MAX_SUPPORTED_COLOR_BUFFERS;
    }

} // namespace Berserk
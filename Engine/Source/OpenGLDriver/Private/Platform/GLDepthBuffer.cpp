//
// Created by Egor Orachyov on 28.02.2019.
//

#include "Platform/GLDepthBuffer.h"
#include "Platform/GLInclude.h"
#include "Platform/GLProfile.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    namespace Resources
    {

        GLDepthBuffer::~GLDepthBuffer()
        {
            mResourceName.nullify();
        }

        void GLDepthBuffer::initialize(const char *name)
        {
            mWidth = 0;
            mHeight = 0;
            mFrameBufferID = 0;
            mReferenceCount = 0;
            new(&mResourceName) CString(name);
        }

        void GLDepthBuffer::addReference()
        {
            mReferenceCount += 1;
        }

        void GLDepthBuffer::release()
        {
            if (mReferenceCount > 0)
            {
                mReferenceCount -= 1;
            }

            if (mReferenceCount == 0 && mFrameBufferID)
            {
#if OPENGL_PROFILE_PLATFORM
                PUSH("GLDepthBuffer: delete [name: '%s']", mResourceName.get());
#endif

                glDeleteFramebuffers(1, &mFrameBufferID);
                mFrameBufferID = 0;
                mDepthBuffer.release();

                delete(&mResourceName);
            }
        }

        uint32 GLDepthBuffer::getReferenceCount()
        {
            return mReferenceCount;
        }

        uint32 GLDepthBuffer::getMemoryUsage()
        {
            return sizeof(GLDepthBuffer);
        }

        const char* GLDepthBuffer::getName()
        {
            return mResourceName.get();
        }

        void GLDepthBuffer::createDepthBuffer(uint32 size)
        {
            if (mFrameBufferID)
            {
                FAIL(false, "An attempt to recreate depth buffer [name: '%s']", mResourceName.get());
            }

            glGenFramebuffers(1, &mFrameBufferID);

            if (!mFrameBufferID)
            {
                FAIL(false, "An attempt to attach buffer to not initialized depth buffer [name: '%s']", mResourceName.get());
            }

            mWidth = size;
            mHeight = size;

            mDepthBuffer.initialize("CubeDepthBuffer");
            mDepthBuffer.create(size, IRenderDriver::DEPTH24);
            mDepthBuffer.setFiltering(IRenderDriver::FILTER_NEAREST, IRenderDriver::FILTER_NEAREST);
            mDepthBuffer.setWrapping(IRenderDriver::WRAP_CLAMP_TO_EDGE);
            mDepthBuffer.setBorderColor(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer.mTextureID, 0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GLDepthBuffer::createDepthBuffer(uint32 width, uint32 height)
        {
            if (mFrameBufferID)
            {
                FAIL(false, "An attempt to recreate depth buffer [name: '%s']", mResourceName.get());
            }

            glGenFramebuffers(1, &mFrameBufferID);

            if (!mFrameBufferID)
            {
                FAIL(false, "An attempt to attach buffer to not initialized depth buffer [name: '%s']", mResourceName.get());
            }

            mWidth = width;
            mHeight = height;

            mDepthBuffer.initialize("DepthBuffer");
            mDepthBuffer.create(mWidth, mHeight, IRenderDriver::DEPTH24);
            mDepthBuffer.setFiltering(IRenderDriver::FILTER_NEAREST, IRenderDriver::FILTER_NEAREST);
            mDepthBuffer.setWrapping(IRenderDriver::WRAP_CLAMP_TO_BORDER);
            mDepthBuffer.setBorderColor(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer.mTextureID, 0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GLDepthBuffer::bindFrameBuffer()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        }

        void GLDepthBuffer::bindDepthBuffer(uint32 textureSlot)
        {
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(GL_TEXTURE_2D, mDepthBuffer.mTextureID);
        }

        void GLDepthBuffer::getSize(uint32 &width, uint32 &height)
        {
            width = mWidth;
            height = mHeight;
        }

        uint32 GLDepthBuffer::getGPUMemoryUsage()
        {
            return mDepthBuffer.getGPUMemoryUsage();
        }

    } // namespace Resources

} // namespace Berserk
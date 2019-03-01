//
// Created by Egor Orachyov on 28.02.2019.
//

#include "Platform/GLDepthBuffer.h"

namespace Berserk
{

    void GLDepthBuffer::initialize(const char *name)
    {
        mFrameBufferID = 0;
        mReferenceCount = 0;
        mResourceName = name;
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

    }

    void GLDepthBuffer::createDepthBuffer(uint32 width, uint32 height)
    {

    }

    void GLDepthBuffer::bindFrameBuffer()
    {

    }

    void GLDepthBuffer::bindDepthBuffer(uint32 textureSlot)
    {

    }

    void GLDepthBuffer::getSize(uint32 &width, uint32 &height)
    {

    }

    uint32 GLDepthBuffer::getGPUMemoryUsage()
    {
        return mDepthBuffer.getGPUMemoryUsage();
    }

} // namespace Berserk
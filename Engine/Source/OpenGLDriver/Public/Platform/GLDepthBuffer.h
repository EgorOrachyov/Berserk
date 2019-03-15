//
// Created by Egor Orachyov on 28.02.2019.
//

#ifndef BERSERK_GLDEPTHBUFFER_H
#define BERSERK_GLDEPTHBUFFER_H

#include "Platform/GLTexture.h"
#include "Platform/IDepthBuffer.h"
#include "Strings/StaticString.h"

namespace Berserk
{

    /**
     * OpenGL platform implementation for depth buffer object
     */
    class GRAPHICS_API GLDepthBuffer : public IDepthBuffer
    {
    public:

        /** @copydoc IResource::initialize() */
        void initialize(const char* name) override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char* getName() override;

    public:

        /** @copydoc IDepthBuffer::createDepthBuffer() */
        void createDepthBuffer(uint32 size) override;

        /** @copydoc IDepthBuffer::createDepthBuffer() */
        void createDepthBuffer(uint32 width, uint32 height) override;

        /** @copydoc IDepthBuffer::bindFrameBuffer() */
        void bindFrameBuffer() override;

        /** @copydoc IDepthBuffer::bindDepthBuffer() */
        void bindDepthBuffer(uint32 textureSlot) override;

        /** @copydoc IDepthBuffer::getSize() */
        void getSize(uint32 &width, uint32 &height) override;

        /** @copydoc IDepthBuffer::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

    private:

        friend class GLBufferManager;

        uint32 mWidth;
        uint32 mHeight;
        uint32 mFrameBufferID;
        uint32 mReferenceCount;

        GLTexture mDepthBuffer;
        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLDEPTHBUFFER_H
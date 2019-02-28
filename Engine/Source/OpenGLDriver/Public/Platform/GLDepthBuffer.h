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
        virtual void createDepthBuffer(uint32 size) = 0;

        /** @copydoc IDepthBuffer::createDepthBuffer() */
        virtual void createDepthBuffer(uint32 width, uint32 height) = 0;

        /** @copydoc IDepthBuffer::linkBuffers() */
        virtual void linkBuffers() = 0;

        /** @copydoc IDepthBuffer::bindFrameBuffer() */
        virtual void bindFrameBuffer() = 0;

        /** @copydoc IDepthBuffer::bindDepthBuffer() */
        virtual void bindDepthBuffer(uint32 textureSlot) = 0;

        /** @copydoc IDepthBuffer::getSize() */
        virtual void getSize(uint32 &width, uint32 &height) = 0;

        /** @copydoc IDepthBuffer::getGPUMemoryUsage() */
        virtual uint32 getGPUMemoryUsage() = 0;

    private:

        uint32 mFrameBufferID;
        uint32 mReferenceCount;

        GLTexture mDepthBuffer;
        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLDEPTHBUFFER_H
//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLFRAMEBUFFER_H
#define BERSERK_GLFRAMEBUFFER_H

#include "Containers/ArrayList.h"
#include "Platform/IFrameBuffer.h"
#include "Strings/StaticString.h"
#include "GLTexture.h"

namespace Berserk
{

    /**
     * OpenGL implementation for GPU frame buffer target object
     */
    class GRAPHICS_API GLFrameBuffer : public IFrameBuffer
    {
    public:

        ~GLFrameBuffer() = default;

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

        /** @copydoc IFrameBuffer::createFrameBuffer() */
        void createFrameBuffer(uint32 width, uint32 height) override;

        /** @copydoc IFrameBuffer::attachColorBuffer() */
        void attachColorBuffer(uint32 slot, IRenderDriver::StorageFormat format) override;

        /** @copydoc IFrameBuffer::attachDepthBuffer() */
        void attachDepthBuffer() override;

        /** @copydoc IFrameBuffer::attachDepthStencilBuffer() */
        void attachDepthStencilBuffer() override;

        /** @copydoc IFrameBuffer::linkBuffers() */
        void linkBuffers() override;

        /** @copydoc IFrameBuffer::bindFrameBuffer() */
        void bindFrameBuffer() override;

        /** @copydoc IFrameBuffer::bindColorBuffer() */
        void bindColorBuffer(uint32 layer, uint32 textureSlot) override;

        /** @copydoc IFrameBuffer::bindDepthBuffer() */
        void bindDepthBuffer(uint32 textureSlot) override;

        /** @copydoc IFrameBuffer::bindDepthStencilBuffer() */
        void bindDepthStencilBuffer(uint32 textureSlot) override;

        /** @copydoc IFrameBuffer::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

        /** @copydoc IFrameBuffer::getMaxSupportedColorBuffers() */
        uint32 getMaxSupportedColorBuffers() override;

    private:

        friend class GLRenderDriver;

        static const uint32 MAX_SUPPORTED_COLOR_BUFFERS = 16;
        static const uint32 DEFAULT_COLOR_BUFFERS = 4;

        uint32 mWidth;
        uint32 mHeigth;
        uint32 mFrameBufferID;
        uint32 mReferenceCount;

        GLTexture mDepthBuffer;
        ArrayList<GLTexture> mColorAttachments;

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLFRAMEBUFFER_H
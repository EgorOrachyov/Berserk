//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLFRAMEBUFFER_H
#define BERSERK_GLFRAMEBUFFER_H

#include "Containers/ArrayList.h"
#include "Strings/Old/String.h"
#include "Platform/IFrameBuffer.h"
#include "Platform/GLTexture.h"

namespace Berserk
{

    namespace Resources
    {

        /**
         * OpenGL implementation for GPU frame buffer target object
         */
        class GRAPHICS_API GLFrameBuffer : public IFrameBuffer
        {
        public:

            ~GLFrameBuffer();

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
            void createFrameBuffer(uint32 width, uint32 height, uint32 colorAttachments) override;

            /** @copydoc IFrameBuffer::attachColorBuffer() */
            void attachColorBuffer(IRenderDriver::StorageFormat format, IRenderDriver::SamplerFilter filtering,
                                   IRenderDriver::SamplerWrapMode wrapping) override;

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

            /** @copydoc IFrameBuffer::getSize() */
            void getSize(uint32 &width, uint32 &height) override;

            /** @copydoc IFrameBuffer::getGPUMemoryUsage() */
            uint32 getGPUMemoryUsage() override;

            /** @copydoc IFrameBuffer::getMaxSupportedColorBuffers() */
            uint32 getMaxSupportedColorBuffers() override;

        private:

            friend class GLRenderDriver;
            friend class GLBufferManager;

            static const uint32 MAX_SUPPORTED_COLOR_BUFFERS = 16;

            uint32 mWidth;
            uint32 mHeight;
            uint32 mFrameBufferID;
            uint32 mReferenceCount;

            GLTexture mDepthBuffer;
            ArrayList<GLTexture> mColorAttachments;

            CString mResourceName;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_GLFRAMEBUFFER_H
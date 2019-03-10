//
// Created by Egor Orachyov on 01.03.2019.
//

#ifndef BERSERK_GLBUFFERMANAGER_H
#define BERSERK_GLBUFFERMANAGER_H

#include "Platform/GLGPUBuffer.h"
#include "Platform/GLDepthBuffer.h"
#include "Platform/GLFrameBuffer.h"
#include "Platform/GLUniformBuffer.h"
#include "Containers/LinkedList.h"
#include "Manager/IBufferManager.h"

namespace Berserk
{

    /**
     * OpenGL platform shader manager implementation
     */
    class ENGINE_API GLBufferManager
    {
    public:

        /** @copydoc IBufferManager::initialize() */
        virtual void initialize() = 0;

        /** @copydoc IBufferManager::release() */
        virtual void release() = 0;

        /** @copydoc IBufferManager::renameGPUBuffer() */
        virtual void renameGPUBuffer(IGPUBuffer* buffer, const char* name) = 0;

        /** @copydoc IBufferManager::renameFrameBuffer() */
        virtual void renameFrameBuffer(IFrameBuffer* buffer, const char* name) = 0;

        /** @copydoc IBufferManager::renameDepthBuffer() */
        virtual void renameDepthBuffer(IDepthBuffer* buffer, const char* name) = 0;

        /** @copydoc IBufferManager::renameUniformBuffer() */
        virtual void renameUniformBuffer(IUniformBuffer* buffer, const char* name) = 0;

        /** @copydoc IBufferManager::IBufferManager() */
        virtual void deleteGPUBuffer(IGPUBuffer* buffer) = 0;

        /** @copydoc IBufferManager::deleteFrameBuffer() */
        virtual void deleteFrameBuffer(IFrameBuffer* buffer) = 0;

        /** @copydoc IBufferManager::deleteDepthBuffer() */
        virtual void deleteDepthBuffer(IDepthBuffer* buffer) = 0;

        /** @copydoc IBufferManager::deleteUniformBuffer() */
        virtual void deleteUniformBuffer(IUniformBuffer* buffer) = 0;

        /** @copydoc IBufferManager::createGPUBuffer() */
        virtual IGPUBuffer* createGPUBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::findGPUBuffer() */
        virtual IGPUBuffer* findGPUBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::getGPUBuffer() */
        virtual IGPUBuffer* getGPUBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::createFrameBuffer() */
        virtual IFrameBuffer* createFrameBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::findFrameBuffer() */
        virtual IFrameBuffer* findFrameBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::getFrameBuffer() */
        virtual IFrameBuffer* getFrameBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::createDepthBuffer() */
        virtual IDepthBuffer* createDepthBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::findDepthBuffer() */
        virtual IDepthBuffer* findDepthBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::getDepthBuffer() */
        virtual IDepthBuffer* getDepthBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::createUniformBuffer() */
        virtual IUniformBuffer* createUniformBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::findUniformBuffer() */
        virtual IUniformBuffer* findUniformBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::getUniformBuffer() */
        virtual IUniformBuffer* getUniformBuffer(const char* name) = 0;

        /** @copydoc IBufferManager::getMemoryUsage() */
        virtual uint32 getMemoryUsage() = 0;

    private:

        /** Number of gpu buffers to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_GPUBUFFERS_COUNT     = 100;

        /** Number of depth buffers to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_DEPTHBUFFERS_COUNT   = 10;

        /** Number of frame buffers to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_FRAMEBUFFERS_COUNT   = 10;

        /** Number of uniform buffers to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_UNIFORMBUFFERS_COUNT = 20;

        LinkedList<GLGPUBuffer>     mGPUBUffers;
        LinkedList<GLDepthBuffer>   mDepthBuffers;
        LinkedList<GLFrameBuffer>   mFrameBuffers;
        LinkedList<GLUniformBuffer> mUniformBuffers;

    };

} // namespace Berserk

#endif //BERSERK_GLBUFFERMANAGER_H
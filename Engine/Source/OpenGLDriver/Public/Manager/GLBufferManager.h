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
     * OpenGL platform bu manager implementation
     */
    class ENGINE_API GLBufferManager : public IBufferManager
    {
    public:

        /** @copydoc IBufferManager::initialize() */
        void initialize() override;

        /** @copydoc IBufferManager::release() */
        void release() override;

        /** @copydoc IBufferManager::renameGPUBuffer() */
        void renameGPUBuffer(IGPUBuffer* buffer, const char* name) override;

        /** @copydoc IBufferManager::renameFrameBuffer() */
        void renameFrameBuffer(IFrameBuffer* buffer, const char* name) override;

        /** @copydoc IBufferManager::renameDepthBuffer() */
        void renameDepthBuffer(IDepthBuffer* buffer, const char* name) override;

        /** @copydoc IBufferManager::renameUniformBuffer() */
        void renameUniformBuffer(IUniformBuffer* buffer, const char* name) override;

        /** @copydoc IBufferManager::IBufferManager() */
        void deleteGPUBuffer(IGPUBuffer* buffer) override;

        /** @copydoc IBufferManager::deleteFrameBuffer() */
        void deleteFrameBuffer(IFrameBuffer* buffer) override;

        /** @copydoc IBufferManager::deleteDepthBuffer() */
        void deleteDepthBuffer(IDepthBuffer* buffer) override;

        /** @copydoc IBufferManager::deleteUniformBuffer() */
        void deleteUniformBuffer(IUniformBuffer* buffer) override;

        /** @copydoc IBufferManager::createGPUBuffer() */
        IGPUBuffer* createGPUBuffer(const char* name) override;

        /** @copydoc IBufferManager::findGPUBuffer() */
        IGPUBuffer* findGPUBuffer(const char* name) override;

        /** @copydoc IBufferManager::getGPUBuffer() */
        IGPUBuffer* getGPUBuffer(const char* name) override;

        /** @copydoc IBufferManager::createFrameBuffer() */
        IFrameBuffer* createFrameBuffer(const char* name) override;

        /** @copydoc IBufferManager::findFrameBuffer() */
        IFrameBuffer* findFrameBuffer(const char* name) override;

        /** @copydoc IBufferManager::getFrameBuffer() */
        IFrameBuffer* getFrameBuffer(const char* name) override;

        /** @copydoc IBufferManager::createDepthBuffer() */
        IDepthBuffer* createDepthBuffer(const char* name) override;

        /** @copydoc IBufferManager::findDepthBuffer() */
        IDepthBuffer* findDepthBuffer(const char* name) override;

        /** @copydoc IBufferManager::getDepthBuffer() */
        IDepthBuffer* getDepthBuffer(const char* name) override;

        /** @copydoc IBufferManager::createUniformBuffer() */
        IUniformBuffer* createUniformBuffer(const char* name) override;

        /** @copydoc IBufferManager::findUniformBuffer() */
        IUniformBuffer* findUniformBuffer(const char* name) override;

        /** @copydoc IBufferManager::getUniformBuffer() */
        IUniformBuffer* getUniformBuffer(const char* name) override;

        /** @copydoc IBufferManager::getMemoryUsage() */
        uint32 getMemoryUsage() override;

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
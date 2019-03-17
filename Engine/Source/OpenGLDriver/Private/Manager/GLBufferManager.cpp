//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Manager/GLBufferManager.h"

namespace Berserk
{

    void GLBufferManager::initialize()
    {
        new(&mGPUBuffers)       LinkedList<GLGPUBuffer>     (INITIAL_GPUBUFFERS_COUNT);
        new(&mFrameBuffers)     LinkedList<GLFrameBuffer>   (INITIAL_FRAMEBUFFERS_COUNT);
        new(&mDepthBuffers)     LinkedList<GLDepthBuffer>   (INITIAL_DEPTHBUFFERS_COUNT);
        new(&mUniformBuffers)   LinkedList<GLUniformBuffer> (INITIAL_UNIFORMBUFFERS_COUNT);
    }

    void GLBufferManager::release()
    {
        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mGPUBuffers.iterate(); current != nullptr; current = mGPUBuffers.next())
            {
                PUSH("GLBufferManager: release gpu buffer [name: '%s']", current->getName());

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mDepthBuffers.iterate(); current != nullptr; current = mDepthBuffers.next())
            {
                PUSH("GLBufferManager: release depth buffer [name: '%s']", current->getName());

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mFrameBuffers.iterate(); current != nullptr; current = mFrameBuffers.next())
            {
                PUSH("GLBufferManager: release frame buffer [name: '%s']", current->getName());

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mUniformBuffers.iterate(); current != nullptr; current = mUniformBuffers.next())
            {
                PUSH("GLBufferManager: release uniform buffer [name: '%s']", current->getName());

                current->mReferenceCount = 0;
                current->release();
            }
        }

        delete(&mGPUBuffers);
        delete(&mDepthBuffers);
        delete(&mFrameBuffers);
        delete(&mUniformBuffers);
    }

    void GLBufferManager::renameGPUBuffer(IGPUBuffer *buffer, const char *name)
    {
        PUSH("GLBufferManager: rename gpu buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);

        auto renamed = dynamic_cast<GLGPUBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameFrameBuffer(IFrameBuffer *buffer, const char *name)
    {
        PUSH("GLBufferManager: rename frame buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);

        auto renamed = dynamic_cast<GLFrameBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameDepthBuffer(IDepthBuffer *buffer, const char *name)
    {
        PUSH("GLBufferManager: rename depth buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);

        auto renamed = dynamic_cast<GLDepthBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameUniformBuffer(IUniformBuffer *buffer, const char *name)
    {
        PUSH("GLBufferManager: rename uniform buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);

        auto renamed = dynamic_cast<GLUniformBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::deleteGPUBuffer(IGPUBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
            PUSH("GLBufferManager: delete buffer [name: '%s']", name.get());
            mGPUBuffers.remove((GLGPUBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteFrameBuffer(IFrameBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
            PUSH("GLBufferManager: delete frame buffer [name: '%s']", name.get());
            mFrameBuffers.remove((GLFrameBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteDepthBuffer(IDepthBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
            PUSH("GLBufferManager: delete depth buffer [name: '%s']", name.get());
            mDepthBuffers.remove((GLDepthBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteUniformBuffer(IUniformBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
            PUSH("GLBufferManager: delete uniform buffer [name: '%s']", name.get());
            mUniformBuffers.remove((GLUniformBuffer*)buffer);
        }
    }

    IGPUBuffer* GLBufferManager::createGPUBuffer(const char *name)
    {
        IGPUBuffer* found = findGPUBuffer(name);

        if (found)
        {
            WARNING("GLBufferManager: gpu buffer already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLGPUBuffer buffer;
            buffer.initialize(name);
            buffer.addReference();

            mGPUBuffers += buffer;

            PUSH("GLBufferManager: create gpu buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
        }

        return mGPUBuffers.getLast();
    }

    IGPUBuffer* GLBufferManager::findGPUBuffer(const char *name)
    {
        for (auto current = mGPUBuffers.iterate(); current != nullptr; current = mGPUBuffers.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IGPUBuffer* GLBufferManager::getGPUBuffer(const char *name)
    {
        IGPUBuffer* found = findGPUBuffer(name);

        if (found)
        {
            found->addReference();
            PUSH("GLBufferManager: find gpu buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
            return found;
        }

        return nullptr;
    }

    IFrameBuffer* GLBufferManager::createFrameBuffer(const char *name)
    {
        IFrameBuffer* found = findFrameBuffer(name);

        if (found)
        {
            WARNING("GLBufferManager: frame buffer already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLFrameBuffer buffer;
            buffer.initialize(name);
            buffer.addReference();

            mFrameBuffers += buffer;

            PUSH("GLBufferManager: create frame buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
        }

        return mFrameBuffers.getLast();
    }

    IFrameBuffer* GLBufferManager::findFrameBuffer(const char *name)
    {
        for (auto current = mFrameBuffers.iterate(); current != nullptr; current = mFrameBuffers.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IFrameBuffer* GLBufferManager::getFrameBuffer(const char *name)
    {
        IFrameBuffer* found = findFrameBuffer(name);

        if (found)
        {
            found->addReference();
            PUSH("GLBufferManager: find frame buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
            return found;
        }

        return nullptr;
    }

    IDepthBuffer* GLBufferManager::createDepthBuffer(const char *name)
    {
        IDepthBuffer* found = findDepthBuffer(name);

        if (found)
        {
            WARNING("GLBufferManager: depth buffer already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLDepthBuffer buffer;
            buffer.initialize(name);
            buffer.addReference();

            mDepthBuffers += buffer;

            PUSH("GLBufferManager: create depth buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
        }

        return mDepthBuffers.getLast();
    }

    IDepthBuffer* GLBufferManager::findDepthBuffer(const char *name)
    {
        for (auto current = mDepthBuffers.iterate(); current != nullptr; current = mDepthBuffers.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IDepthBuffer* GLBufferManager::getDepthBuffer(const char *name)
    {
        IDepthBuffer* found = findDepthBuffer(name);

        if (found)
        {
            found->addReference();
            PUSH("GLBufferManager: find depth buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
            return found;
        }

        return nullptr;
    }

    IUniformBuffer* GLBufferManager::createUniformBuffer(const char *name)
    {
        IUniformBuffer* found = findUniformBuffer(name);

        if (found)
        {
            WARNING("GLBufferManager: uniform buffer already exist [name: '%s']", name);
            return nullptr;
        }

        {
            GLUniformBuffer buffer;
            buffer.initialize(name);
            buffer.addReference();

            mUniformBuffers += buffer;

            PUSH("GLBufferManager: create uniform buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
        }

        return mUniformBuffers.getLast();
    }

    IUniformBuffer* GLBufferManager::findUniformBuffer(const char *name)
    {
        for (auto current = mUniformBuffers.iterate(); current != nullptr; current = mUniformBuffers.next())
        {
            if (current->mResourceName == name)
            {
                return current;
            }
        }

        return nullptr;
    }

    IUniformBuffer* GLBufferManager::getUniformBuffer(const char *name)
    {
        IUniformBuffer* found = findUniformBuffer(name);

        if (found)
        {
            found->addReference();
            PUSH("GLBufferManager: find uniform buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
            return found;
        }

        return nullptr;
    }

    uint32 GLBufferManager::getMemoryUsage()
    {
        return sizeof(GLBufferManager)          +
               mGPUBuffers.getMemoryUsage()     +
               mFrameBuffers.getMemoryUsage()   +
               mDepthBuffers.getMemoryUsage()   +
               mUniformBuffers.getMemoryUsage() ;


    }

} // namespace Berserk
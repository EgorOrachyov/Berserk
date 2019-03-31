//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Managers/GLBufferManager.h"
#include "Platform/GLProfile.h"

namespace Berserk::Resources
{

    GLBufferManager::GLBufferManager() : mGPUBuffers(INITIAL_GPUBUFFERS_COUNT),
                                         mFrameBuffers(INITIAL_FRAMEBUFFERS_COUNT),
                                         mDepthBuffers(INITIAL_DEPTHBUFFERS_COUNT),
                                         mUniformBuffers(INITIAL_UNIFORMBUFFERS_COUNT)
    {
        PUSH("GLBufferManager: initialize");
    }

    GLBufferManager::~GLBufferManager()
    {
        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mGPUBuffers.iterate(); current != nullptr; current = mGPUBuffers.next())
            {
#if PROFILE_GL_BUFFER_MANAGER
                PUSH("GLBufferManager: release gpu buffer [name: '%s']", current->getName());
#endif
                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mDepthBuffers.iterate(); current != nullptr; current = mDepthBuffers.next())
            {
#if PROFILE_GL_BUFFER_MANAGER
                PUSH("GLBufferManager: release depth buffer [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mFrameBuffers.iterate(); current != nullptr; current = mFrameBuffers.next())
            {
#if PROFILE_GL_BUFFER_MANAGER
                PUSH("GLBufferManager: release frame buffer [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        {
            // Explicit release delete for non
            // deleted buffer resources (Avoids memory leaks)

            for (auto current = mUniformBuffers.iterate(); current != nullptr; current = mUniformBuffers.next())
            {
#if PROFILE_GL_BUFFER_MANAGER
                PUSH("GLBufferManager: release uniform buffer [name: '%s']", current->getName());
#endif

                current->mReferenceCount = 0;
                current->release();
            }
        }

        PUSH("GLBufferManager: de-initialize");
    }

    void GLBufferManager::renameGPUBuffer(IGPUBuffer *buffer, const char *name)
    {
#if PROFILE_GL_BUFFER_MANAGER
        PUSH("GLBufferManager: rename gpu buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);
#endif

        auto renamed = dynamic_cast<GLGPUBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameFrameBuffer(IFrameBuffer *buffer, const char *name)
    {
#if PROFILE_GL_BUFFER_MANAGER
        PUSH("GLBufferManager: rename frame buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);
#endif

        auto renamed = dynamic_cast<GLFrameBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameDepthBuffer(IDepthBuffer *buffer, const char *name)
    {
#if PROFILE_GL_BUFFER_MANAGER
        PUSH("GLBufferManager: rename depth buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);
#endif

        auto renamed = dynamic_cast<GLDepthBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::renameUniformBuffer(IUniformBuffer *buffer, const char *name)
    {
#if PROFILE_GL_BUFFER_MANAGER
        PUSH("GLBufferManager: rename uniform buffer [old name: '%s'][new name: '%s']", buffer->getName(), name);
#endif

        auto renamed = dynamic_cast<GLUniformBuffer*>(buffer);
        renamed->mResourceName = name;
    }

    void GLBufferManager::deleteGPUBuffer(IGPUBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: delete buffer [name: '%s']", name.get());
#endif
            mGPUBuffers.remove((GLGPUBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteFrameBuffer(IFrameBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: delete frame buffer [name: '%s']", name.get());
#endif
            mFrameBuffers.remove((GLFrameBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteDepthBuffer(IDepthBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: delete depth buffer [name: '%s']", name.get());
#endif
            mDepthBuffers.remove((GLDepthBuffer*)buffer);
        }
    }

    void GLBufferManager::deleteUniformBuffer(IUniformBuffer *buffer)
    {
        CText name(buffer->getName());
        buffer->release();

        if (buffer->getReferenceCount() == 0)
        {
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: delete uniform buffer [name: '%s']", name.get());
#endif
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

#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: create gpu buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
#endif
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
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: find gpu buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
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

#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: create frame buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
#endif
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
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: find frame buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
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

#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: create depth buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
#endif
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
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: find depth buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
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

#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: create uniform buffer [name: '%s'][ref: %u]", buffer.getName(), buffer.getReferenceCount());
#endif
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
#if PROFILE_GL_BUFFER_MANAGER
            PUSH("GLBufferManager: find uniform buffer [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
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

} // namespace Berserk::Resources
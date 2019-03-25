//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Platform/GLProfile.h"
#include "Platform/GLUniformBuffer.h"
#include "Platform/GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    namespace Resources
    {

        GLUniformBuffer::~GLUniformBuffer()
        {
            mResourceName.nullify();
        }

        void GLUniformBuffer::initialize(const char *name)
        {
            mBufferID = 0;
            mBufferSize = 0;
            mBindingPoint = 0;
            mReferenceCount = 0;
            new(&mResourceName) CString(name);
        }

        void GLUniformBuffer::addReference()
        {
            mReferenceCount += 1;
        }

        void GLUniformBuffer::release()
        {
            if (mReferenceCount > 0)
            {
                mReferenceCount -= 1;
            }

            if (mReferenceCount == 0 && mBufferID)
            {
#if PROFILE_GL_PLATFORM
                PUSH("GLUniformBuffer: delete [name: '%s']", mResourceName.get());
#endif

                glDeleteBuffers(1, &mBufferID);
                mBufferID = 0;

                delete(&mResourceName);
            }
        }

        uint32 GLUniformBuffer::getReferenceCount()
        {
            return mReferenceCount;
        }

        uint32 GLUniformBuffer::getMemoryUsage()
        {
            return sizeof(GLUniformBuffer);
        }

        const char* GLUniformBuffer::getName()
        {
            return mResourceName.get();
        }

        void GLUniformBuffer::create(uint32 bindingPoint, uint32 size, const void *data)
        {
            if (mBufferID)
            {
                FAIL(false, "An attempt to recreate uniform buffer [name: '%s']", mResourceName.get());
            }

            glGenBuffers(1, &mBufferID);
            glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
            glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            mBufferSize = size;
            mBindingPoint = bindingPoint;
        }

        void GLUniformBuffer::update(uint32 size, const void *data)
        {
            FAIL(size <= mBufferSize, "Buffer size is less than data size");

            glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
            glBufferSubData(GL_UNIFORM_BUFFER, 0 , size, data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void GLUniformBuffer::update(uint32 offset, uint32 size, const void *data)
        {
            FAIL(offset + size <= mBufferSize, "Buffer size is less than data size");

            glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
            glBufferSubData(GL_UNIFORM_BUFFER, offset , size, data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void GLUniformBuffer::bind(uint32 bindingPoint)
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, mBufferID);
        }

        void GLUniformBuffer::bind()
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mBufferID);
        }

        void GLUniformBuffer::setBindingPoint(uint32 bindingPoint)
        {
            mBindingPoint = bindingPoint;
        }

        uint32 GLUniformBuffer::getBindingPoint()
        {
            return mBindingPoint;
        }

        uint32 GLUniformBuffer::getGPUMemoryUsage()
        {
            return mBufferSize;
        }

    } // namespace Resources

} // namespace Berserk
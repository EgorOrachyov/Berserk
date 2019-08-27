//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLUniformBuffer.h>

namespace Berserk
{

    GLUniformBuffer::GLUniformBuffer(uint32 bindingPoint, GLenum bufferUsage, Berserk::uint32 size, const Berserk::uint8 *data)
        : mBufferSize(size), mBindingPoint(bindingPoint)
    {
        glGenBuffers(1, &mResourceID);
        glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
        glBufferData(GL_UNIFORM_BUFFER, size, data, bufferUsage);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    GLUniformBuffer::~GLUniformBuffer()
    {
        if (mResourceID)
        {
            glDeleteBuffers(1, &mResourceID);
            mResourceID = 0;
        }
    }

    void GLUniformBuffer::update(uint32 size, const uint8 *data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
        glBufferSubData(GL_UNIFORM_BUFFER, 0 , size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::update(uint32 size, uint32 offset, const uint8 *data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mResourceID);
        glBufferSubData(GL_UNIFORM_BUFFER, offset , size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::bind(uint32 bindingPoint) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, mResourceID);
    }

    void GLUniformBuffer::bind() const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mResourceID);
    }

    void GLUniformBuffer::setBindingPoint(uint32 bindingPoint)
    {
        mBindingPoint = bindingPoint;
    }

    uint32 GLUniformBuffer::getBindingPoint() const
    {
        return mBindingPoint;
    }

    uint32 GLUniformBuffer::getSize() const
    {
        return mBufferSize;
    }

    uint32 GLUniformBuffer::getMemoryUsage() const
    {
        return sizeof(GLUniformBuffer);
    }

    uint32 GLUniformBuffer::getMemoryUsage_GPU() const
    {
        return mBufferSize;
    }

} // namespace Berserk
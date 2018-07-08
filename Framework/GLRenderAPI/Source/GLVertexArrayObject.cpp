//
// Created by Egor Orachyov on 04.07.2018.
//

#include "Buffers/GLVertexArrayObject.h"

#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLVertexArrayObject::GLVertexArrayObject()
    {
        mHandle = 0;
        mBuffer = 0;
        mCount = 0;

        mUseEBO = false;
    }

    GLVertexArrayObject::~GLVertexArrayObject()
    {
        destroy();
    }

    void GLVertexArrayObject::init()
    {
        glGenVertexArrays(1, &mHandle);
    }

    void GLVertexArrayObject::destroy()
    {
        if (mHandle)
        {
            glDeleteVertexArrays(1, &mHandle);

            if (mBuffer)
            {
                glDeleteBuffers(1, &mBuffer);
            }
        }

        mHandle = 0;
        mBuffer = 0;
        mCount = 0;
    }

    void GLVertexArrayObject::draw()
    {
        glBindVertexArray(mHandle);

        if (mUseEBO)
            glDrawElements(mMode, mCount, mType, NULL);
        else
            glDrawArrays(mMode, 0, mCount);
    }

    void GLVertexArrayObject::draw(uint32 count, GLPrimitiveMode mode) const
    {
        ASSERT(count > 0, "An attempt to pass negative count of vertexes");
        glBindVertexArray(mHandle);
        glDrawArrays(mode, 0, count);
    }

    void GLVertexArrayObject::attachBuffer(GLDataBufferPacker& packer)
    {
        if (packer.isPacked() && mHandle && !mBuffer)
        {
            // Bind our VAO
            glBindVertexArray(mHandle);

            // Create and fill buffer
            glGenBuffers(1, &mBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
            glBufferData(GL_ARRAY_BUFFER, packer.getTotalBufferSize(), packer.getBuffer(), GL_STATIC_DRAW);

            for (uint32 i = 0; i < packer.getBuffersCount(); i++)
            {
                GLDataBufferPacker::VertexData& data = packer.mMetaData.get(i);

                glVertexAttribPointer(data.index, data.perVertCount, data.type, data.normalized, packer.mStride, data.offset);
                glEnableVertexAttribArray(data.index);
            }
        }
        else
        {
            WARNING("Cannot attach packer data");
        }
    }

    void GLVertexArrayObject::attachBuffer(GLVertexBufferObject& buffer)
    {

    }

    void GLVertexArrayObject::attachBuffer(GLElementBufferObject& buffer)
    {
        if (mHandle)
        {
            if (buffer.isCreated())
            {
                glBindVertexArray(mHandle);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.mHandle);

                mCount = buffer.mCount;
                mMode = buffer.mMode;
                mType = buffer.mType;
                mUseEBO = true;
            }
            else
            {
                WARNING("An attempt to attach not initialized EBO to VAO");
            }
        }
        else
        {
            WARNING("An attempt to attach EBO to not initialized VAO");
        }
    }

    void GLVertexArrayObject::attachBuffer(GLDataBufferPacker& packer, GLElementBufferObject& buffer)
    {
        if (packer.isPacked() && mHandle && !mBuffer)
        {
            // Bind our VAO
            glBindVertexArray(mHandle);

            // Create and fill buffer
            glGenBuffers(1, &mBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
            glBufferData(GL_ARRAY_BUFFER, packer.getTotalBufferSize(), packer.getBuffer(), GL_STATIC_DRAW);

            if (mHandle)
            {
                if (buffer.isCreated())
                {
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.mHandle);

                    mCount = buffer.mCount;
                    mMode = buffer.mMode;
                    mType = buffer.mType;
                    mUseEBO = true;
                }
                else
                {
                    WARNING("An attempt to attach not initialized EBO to VAO");
                }
            }
            else
            {
                WARNING("An attempt to attach EBO to not initialized VAO");
            }

            for (uint32 i = 0; i < packer.getBuffersCount(); i++)
            {
                GLDataBufferPacker::VertexData& data = packer.mMetaData.get(i);

                glVertexAttribPointer(data.index, data.perVertCount, data.type, data.normalized, packer.mStride, data.offset);
                glEnableVertexAttribArray(data.index);
            }

        }
        else
        {
            WARNING("Cannot attach packer data");
        }
    }

    void GLVertexArrayObject::detach()
    {

    }

    void GLVertexArrayObject::setDrawingMode(uint32 count, GLPrimitiveMode mode)
    {
        if (count > 0)
        {
            mCount = count;
            mMode = mode;
        }
        else
        {
            WARNING("An attempt to pass negative count of vertexes");
        }
    }

    GLuint GLVertexArrayObject::getHandle() const
    {
        return mHandle;
    }

    GLuint GLVertexArrayObject::getBuffer() const
    {
        return mBuffer;
    }

    GLint GLVertexArrayObject::getCount() const
    {
        return mCount;
    }

    GLPrimitiveMode GLVertexArrayObject::getMode() const
    {
        return mMode;
    }

} // namespace Berserk
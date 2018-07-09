//
// Created by Egor Orachyov on 09.07.2018.
//

#include "Buffers/GLGPUBuffer.h"
#include "../../Core/Logging/LogMessages.h"

namespace Berserk
{

    GLGPUBuffer::GLGPUBuffer()
    {
        mVAOHandle = 0;
        mVBOHandle = 0;
        mEBOHandle = 0;
        mCount = 0;
    }

    GLGPUBuffer::~GLGPUBuffer()
    {
        destroy();
    }

    void GLGPUBuffer::init()
    {
        ASSERT(!mVAOHandle, "An attempt to init already created buffer");

        if (!isInitialized())
        {
            glGenVertexArrays(1, &mVAOHandle);
        }
    }

    void GLGPUBuffer::destroy()
    {
        PUSH("Delete GLGPUBuffer %p\n", this);

        if (isInitialized())
        {
            PUSH("Delete VAO %u\n", mVAOHandle);
            glDeleteVertexArrays(1, &mVAOHandle);
        }
        if (isDataAtteched())
        {
            PUSH("Delete VBO %u\n", mVBOHandle);
            glDeleteBuffers(1, &mVBOHandle);
        }
        if (isIndicesAttached())
        {
            PUSH("Delete EBO %u\n", mEBOHandle);
            glDeleteBuffers(1, &mEBOHandle);
        }

        mVAOHandle = 0;
        mVBOHandle = 0;
        mEBOHandle = 0;
        mCount = 0;
    }

    void GLGPUBuffer::drawData() const
    {
        glBindVertexArray(mVAOHandle);
        glDrawArrays(mMode, 0, mCount);
    }

    void GLGPUBuffer::drawIndices() const
    {
        glBindVertexArray(mVAOHandle);
        glDrawElements(mMode, mCount, mIndicesType, NULL);
    }

    void GLGPUBuffer::setDrawingProperties(uint32 count, GLPrimitiveMode mode)
    {
        mCount = count;
        mMode = mode;
    }

    void GLGPUBuffer::setDrawingProperties(uint32 count, GLPrimitiveMode mode, GLDataType indexType)
    {
        mCount = count;
        mMode = mode;
        mIndicesType = indexType;
    }

    void GLGPUBuffer::attachData(GLDataBufferPacker& packer)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAtteched())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }
        if (!packer.isPacked())
        {
            WARNING("Cannot attach unpacked data to this GLGPUBuffer");
            return;
        }

        // Bind our VAO
        glBindVertexArray(mVAOHandle);

        // Create and fill buffer
        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, packer.getTotalBufferSize(), packer.getBuffer(), GL_STATIC_DRAW);

        // Set up vertex attributes
        for (uint32 i = 0; i < packer.getBuffersCount(); i++)
        {
            GLDataBufferPacker::VertexData& data = packer.mMetaData.get(i);
            glVertexAttribPointer(data.index, data.perVertCount, data.type, data.normalized, packer.mStride, data.offset);
            glEnableVertexAttribArray(data.index);
        }

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachIndices(uint32 count, uint16* indices, GLPrimitiveMode mode)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isIndicesAttached())
        {
            WARNING("Indices data has been attached to this GLGPUBuffer");
            return;
        }

        // Bind our VAO
        glBindVertexArray(mVAOHandle);

        // Gen EBO and fill buffer
        glGenBuffers(1, &mEBOHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBOHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * count, indices, GL_STATIC_DRAW);

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Save properties
        mMode = mode;
        mCount = count;
        mIndicesType = GLPT_USHORT;
    }

    void GLGPUBuffer::attachIndices(uint32 count, uint32* indices, GLPrimitiveMode mode)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isIndicesAttached())
        {
            WARNING("Indices data has been attached to this GLGPUBuffer");
            return;
        }

        // Bind our VAO
        glBindVertexArray(mVAOHandle);

        // Gen EBO and fill buffer
        glGenBuffers(1, &mEBOHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBOHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count, indices, GL_STATIC_DRAW);

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Save properties
        mMode = mode;
        mCount = count;
        mIndicesType = GLPT_UINT;
    }

    bool GLGPUBuffer::isInitialized() const
    {
        return (mVAOHandle != 0);
    }

    bool GLGPUBuffer::isDataAtteched() const
    {
        return (mVBOHandle != 0);
    }

    bool GLGPUBuffer::isIndicesAttached() const
    {
        return (mEBOHandle != 0);
    }

    bool GLGPUBuffer::validate() const
    {
        return (mVAOHandle && mVBOHandle && mEBOHandle);
    }

    uint32 GLGPUBuffer::getVAO() const
    {
        return mVAOHandle;
    }

    uint32 GLGPUBuffer::getVBO() const
    {
        return mVBOHandle;
    }

    uint32 GLGPUBuffer::getEBO() const
    {
        return mEBOHandle;
    }

    uint32 GLGPUBuffer::getCount() const
    {
        return mCount;
    }

    GLPrimitiveMode GLGPUBuffer::getMode() const
    {
        return mMode;
    }

    GLDataType GLGPUBuffer::getIndicesType() const
    {
        return mIndicesType;
    }
    
} // namespace Berserk
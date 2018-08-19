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

    UINT32 GLGPUBuffer::getMemoryUsage()
    {
        return 0 /* todo */;
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
        //PUSH("GLGPUBuffer: delete %p", this);

        if (isInitialized())
        {
            //PUSH("Delete VAO %u", mVAOHandle);
            glDeleteVertexArrays(1, &mVAOHandle);
        }
        if (isDataAttached())
        {
            //PUSH("Delete VBO %u", mVBOHandle);
            glDeleteBuffers(1, &mVBOHandle);
        }
        if (isIndicesAttached())
        {
            //PUSH("Delete EBO %u", mEBOHandle);
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

    void GLGPUBuffer::setDrawingProperties(UINT32 count, GLPrimitiveMode mode)
    {
        mCount = count;
        mMode = mode;
    }

    void GLGPUBuffer::setDrawingProperties(UINT32 count, GLPrimitiveMode mode, GLDataType indexType)
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
        if (isDataAttached())
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
        for (UINT32 i = 0; i < packer.getBuffersCount(); i++)
        {
            GLDataBufferPacker::VertexData& data = packer.mMetaData.get(i);
            glVertexAttribPointer(data.index, data.perVertCount, data.type, data.normalized, packer.mStride, data.offset);
            glEnableVertexAttribArray(data.index);
        }

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(ArrayList<Vertex> &vertices)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, vertices.getElementSize() * vertices.getSize(), vertices.getBuffer(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(ArrayList<VertexPN> &vertices)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, vertices.getElementSize() * vertices.getSize(), vertices.getBuffer(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPN), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPN), (void*) sizeof(Vector3f));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(ArrayList<VertexPT> &vertices)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, vertices.getElementSize() * vertices.getSize(), vertices.getBuffer(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPT), (void*) sizeof(Vector3f));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(ArrayList<VertexPNT> &vertices)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, vertices.getElementSize() * vertices.getSize(), vertices.getBuffer(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*) sizeof(Vector3f));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*) (2 * sizeof(Vector3f)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(ArrayList<VertexPNTBT> &vertices)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, vertices.getElementSize() * vertices.getSize(), vertices.getBuffer(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) sizeof(Vector3f));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (2 * sizeof(Vector3f)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (2 * sizeof(Vector3f) + sizeof(Vector2f)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (3 * sizeof(Vector3f) + sizeof(Vector2f)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(Vertex* data, UINT32 count)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(VertexPN* data, UINT32 count)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * count, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPN), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPN), (void*) sizeof(Vector3f));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(VertexPT* data, UINT32 count)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPT) * count, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPT), (void*) sizeof(Vector3f));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(VertexPNT* data, UINT32 count)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNT) * count, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*) sizeof(Vector3f));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNT), (void*) (2 * sizeof(Vector3f)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachData(VertexPNTBT* data, UINT32 count)
    {
        if (!isInitialized())
        {
            WARNING("GLGPUBuffer is not initialized");
            return;
        }
        if (isDataAttached())
        {
            WARNING("Data has been attached to this GLGPUBuffer");
            return;
        }

        glBindVertexArray(mVAOHandle);

        glGenBuffers(1, &mVBOHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNTBT) * count, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) sizeof(Vector3f));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (2 * sizeof(Vector3f)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (2 * sizeof(Vector3f) + sizeof(Vector2f)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GLDataType::GLDT_FLOAT,
                              GLNormalization::GLN_DO_NOT_USE, sizeof(VertexPNTBT), (void*) (3 * sizeof(Vector3f) + sizeof(Vector2f)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLGPUBuffer::attachIndices(ArrayList<UINT16> &indices)
    {
        attachIndices(indices.getSize(), indices.getBuffer(), GLPrimitiveMode::GLPM_TRIANGLES);
    }

    void GLGPUBuffer::attachIndices(ArrayList<UINT32> &indices)
    {
        attachIndices(indices.getSize(), indices.getBuffer(), GLPrimitiveMode::GLPM_TRIANGLES);
    }

    void GLGPUBuffer::attachIndices(UINT16 *indices, UINT32 count)
    {
        attachIndices(count, indices, GLPrimitiveMode::GLPM_TRIANGLES);
    }

    void GLGPUBuffer::attachIndices(UINT32 *indices, UINT32 count)
    {
        attachIndices(count, indices, GLPrimitiveMode::GLPM_TRIANGLES);
    }

    void GLGPUBuffer::attachIndices(UINT32 count, UINT16* indices, GLPrimitiveMode mode)
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UINT16) * count, indices, GL_STATIC_DRAW);

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Save properties
        mMode = mode;
        mCount = count;
        mIndicesType = GLDT_USHORT;
    }

    void GLGPUBuffer::attachIndices(UINT32 count, UINT32* indices, GLPrimitiveMode mode)
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UINT32) * count, indices, GL_STATIC_DRAW);

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Save properties
        mMode = mode;
        mCount = count;
        mIndicesType = GLDT_UINT;
    }

    bool GLGPUBuffer::isInitialized() const
    {
        return (mVAOHandle != 0);
    }

    bool GLGPUBuffer::isDataAttached() const
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

    UINT32 GLGPUBuffer::getVAO() const
    {
        return mVAOHandle;
    }

    UINT32 GLGPUBuffer::getVBO() const
    {
        return mVBOHandle;
    }

    UINT32 GLGPUBuffer::getEBO() const
    {
        return mEBOHandle;
    }

    UINT32 GLGPUBuffer::getCount() const
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
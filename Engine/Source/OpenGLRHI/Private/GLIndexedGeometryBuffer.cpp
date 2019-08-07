//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLResources.h>
#include <Logging/DebugLogMacros.h>
#include <Rendering/VertexTypes.h>

namespace Berserk
{

    GLIndexedGeometryBuffer::GLIndexedGeometryBuffer(const RHIVertexBufferRef &vertexBuffer,
                                                     const RHIIndexBufferRef &indexBuffer,
                                                     EDataLayout dataLayout, EPrimitiveType primitiveType,
                                                     uint32 indicesCount,
                                                     GLenum primitiveType_gl, GLenum indexType_gl)
            : mVertexBuffer(vertexBuffer),
              mIndexBuffer(indexBuffer),
              mPrimitiveType(primitiveType),
              mIndicesCount(indicesCount),
              mGL_indicesType(indexType_gl),
              mGL_primitiveType(primitiveType_gl)
    {
        GLuint VBO = ((GLVertexBuffer*)vertexBuffer.operator->())->mResourceID;
        GLuint EBO = ((GLVertexBuffer*)indexBuffer.operator->())->mResourceID;

        glGenVertexArrays(1, &mResourceID);
        glBindVertexArray(mResourceID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        switch (dataLayout)
        {
            case EDataLayout::DL_Vertex:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(Vertf), (void*)0);
                break;

            case EDataLayout::DL_VertexPN:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNf), (void*) sizeof(Vec3f));
                break;

            case EDataLayout::DL_VertexPT:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPTf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPTf), (void*) sizeof(Vec3f));
                break;

            case EDataLayout::DL_VertexPNT:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTf), (void*) sizeof(Vec3f));

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTf), (void*) (2 * sizeof(Vec3f)));
                break;

            case EDataLayout::DL_VertexPNTBT:
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTBTf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTBTf), (void*) sizeof(Vec3f));

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTBTf), (void*) (2 * sizeof(Vec3f)));

                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTBTf), (void*) (3 * sizeof(Vec3f)));

                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 2, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNTBTf), (void*) (4 * sizeof(Vec3f)));
                break;

            default:
                DEBUG_LOG_ERROR("GLIndexedGeometryBuffer: invalid data layout [enum: %u]", dataLayout);
                return;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    GLIndexedGeometryBuffer::~GLIndexedGeometryBuffer()
    {
        if (mResourceID)
        {
            glDeleteVertexArrays(1, &mResourceID);
            mResourceID = 0;
        }
    }

    void GLIndexedGeometryBuffer::draw()
    {
        glBindVertexArray(mResourceID);
        glDrawElements(mGL_primitiveType, mIndicesCount, mGL_indicesType, nullptr);
    }

    void GLIndexedGeometryBuffer::draw(uint32 numOfElements)
    {
        glBindVertexArray(mResourceID);
        glDrawElements(mGL_primitiveType, numOfElements, mGL_indicesType, nullptr);
    }

    void GLIndexedGeometryBuffer::draw(uint32 numOfIndices, uint32 offsetBuffer)
    {
        glBindVertexArray(mResourceID);
        glDrawElements(mGL_primitiveType, numOfIndices, mGL_indicesType, (void*) offsetBuffer);
    }

    EPrimitiveType GLIndexedGeometryBuffer::getPrimitiveType() const
    {
        return mPrimitiveType;
    }

    uint32 GLIndexedGeometryBuffer::getMemoryUsage() const
    {
        return sizeof(GLIndexedGeometryBuffer);
    }

    uint32 GLIndexedGeometryBuffer::getMemoryUsage_GPU() const
    {
        return sizeof(GLIndexedGeometryBuffer);
    }

} // namespace Berserk
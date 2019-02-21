//
// Created by Egor Orachyov on 07.02.2019.
//

#include "GLInclude.h"
#include "GLTexture.h"
#include "GLGPUBuffer.h"
#include "Logging/LogMacros.h"
#include "Platform/VertexTypes.h"

namespace Berserk
{

    void GLGPUBuffer::initialize(const char *name)
    {
        mVertexArrayObject = 0;
        mElementBufferObject = 0;
        mVertexBufferObject = 0;

        mReferenceCount = 0;
        mIndicesCount = 0;
        mVerticesCount = 0;

        mResourceName = name;
    }

    void GLGPUBuffer::addReference()
    {
        mReferenceCount += 1;
    }

    void GLGPUBuffer::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0)
        {
            PUSH("GLGPUBuffer: delete | name : %s | vao: %u | ebo: %u | vbo: %u",
                 mResourceName.get(), mVertexArrayObject, mElementBufferObject, mVertexBufferObject);

            if (mVertexArrayObject) glDeleteVertexArrays(1, &mVertexArrayObject);
            if (mElementBufferObject) glDeleteBuffers(1, &mElementBufferObject);
            if (mVertexBufferObject) glDeleteBuffers(1, &mVertexBufferObject);

            mVertexArrayObject   = 0;
            mElementBufferObject = 0;
            mVertexBufferObject  = 0;
        }

    }

    uint32 GLGPUBuffer::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLGPUBuffer::getMemoryUsage()
    {
        return sizeof(GLGPUBuffer);
    }

    const char* GLGPUBuffer::getName()
    {
        return mResourceName.get();
    }

    void GLGPUBuffer::create(uint32 verticesCount,
                             VertexType vertexType,
                             void *vertices,
                             uint32 indicesCount,
                             uint16 *indices)
    {
        glGenVertexArrays(1, &mVertexArrayObject);
        glBindVertexArray(mVertexArrayObject);

        glGenBuffers(1, &mVertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);

        if (vertexType == Vertex)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertf) * verticesCount, vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(Vertf), (void*)0);
        }
        else if (vertexType == VertexPN)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNf) * verticesCount, vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPNf), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPNf), (void*) sizeof(Vec3f));
        }
        else if (vertexType == VertexPT)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(VertPTf) * verticesCount, vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPTf), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPTf), (void*) sizeof(Vec3f));
        }
        else if (vertexType == VertexPNT)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNTf) * verticesCount, vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPNTf), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPNTf), (void*) sizeof(Vec3f));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT,
                                  GL_FALSE, sizeof(VertPNTf), (void*) (2 * sizeof(Vec3f)));
        }
        else if (vertexType == VertexPNTBT)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNTBTf) * verticesCount, vertices, GL_STATIC_DRAW);

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
        }
        else
        {
            FAIL(false, "Unknown vertex format");
        }

        glGenBuffers(1, &mElementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * indicesCount, indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        mIndicesCount = indicesCount;
        mVerticesCount = verticesCount;
        mPrimitiveMode = TRIANGLES;
        mVertexType = vertexType;
        mIndicesType = ITexture::UNSIGNED_SHORT;
    }

    void GLGPUBuffer::setDrawingProperties(uint32 count,
                                           PrimitiveMode mode,
                                           ITexture::PixelType indicesType)
    {
        mIndicesCount = count;
        mPrimitiveMode = mode;
        mIndicesType = indicesType;
    }

    void GLGPUBuffer::draw()
    {
        glBindVertexArray(mVertexArrayObject);
        glDrawElements(getPrimitiveMode(mPrimitiveMode), mIndicesCount, GLTexture::getPixelType(mIndicesType), nullptr);
    }

    IGPUBuffer::VertexType GLGPUBuffer::getVertexType()
    {
        return mVertexType;
    }

    IGPUBuffer::PrimitiveMode GLGPUBuffer::getPrimitiveMode()
    {
        return mPrimitiveMode;
    }

    uint32 GLGPUBuffer::getGPUMemoryUsage()
    {
        return 0;
    }

    uint32 GLGPUBuffer::getPrimitiveMode(PrimitiveMode mode)
    {
        switch (mode)
        {
            case TRIANGLES:
                return GL_TRIANGLES;

            case LINES:
                return GL_LINES;

            case LINE_STRIP:
                return GL_LINE_STRIP;

            case LINE_LOOP:
                return GL_LINE_LOOP;

            case POINTS:
                return GL_POINTS;

            default:
                WARNING("Invalid argument value");
        }

        return 0;
    }

} // namespace Berserk
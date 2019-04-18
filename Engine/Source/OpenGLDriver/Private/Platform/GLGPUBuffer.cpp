//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Platform/GLProfile.h"
#include "Platform/GLInclude.h"
#include "Platform/GLTexture.h"
#include "Platform/GLGPUBuffer.h"
#include "Platform/GLRenderDriver.h"
#include "Logging/LogMacros.h"
#include "Platform/VertexTypes.h"

namespace Berserk
{

    namespace Resources
    {

        GLGPUBuffer::~GLGPUBuffer()
        {
            mResourceName.nullify();
        }

        void GLGPUBuffer::initialize(const char *name)
        {
            mVertexArrayObject = 0;
            mElementBufferObject = 0;
            mVertexBufferObject = 0;

            mReferenceCount = 0;
            mIndicesCount = 0;
            mVerticesCount = 0;

            new(&mResourceName) CString(name);
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
#if PROFILE_GL_PLATFORM
                PUSH("GLGPUBuffer: delete [name: '%s']", mResourceName.get());
#endif

                if (mVertexArrayObject) glDeleteVertexArrays(1, &mVertexArrayObject);
                if (mElementBufferObject) glDeleteBuffers(1, &mElementBufferObject);
                if (mVertexBufferObject) glDeleteBuffers(1, &mVertexBufferObject);

                mVertexArrayObject   = 0;
                mElementBufferObject = 0;
                mVertexBufferObject  = 0;

                delete(&mResourceName);
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
            create(verticesCount, vertexType, vertices, indicesCount, indices, IRenderDriver::USAGE_STATIC_DRAW);
        }

        void GLGPUBuffer::create(uint32 verticesCount,
                                 VertexType vertexType,
                                 void *vertices,
                                 uint32 indicesCount,
                                 uint16 *indices,
                                 IRenderDriver::BufferUsage usage)
        {
            glGenVertexArrays(1, &mVertexArrayObject);
            glBindVertexArray(mVertexArrayObject);

            glGenBuffers(1, &mVertexBufferObject);
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);

            auto draw = GLRenderDriver::getBufferUsage(usage);

            if (vertexType == eVT_Vertex)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertf) * verticesCount, vertices, draw);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(Vertf), (void*)0);
            }
            else if (vertexType == eVT_VertexPN)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNf) * verticesCount, vertices, draw);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPNf), (void*) sizeof(Vec3f));
            }
            else if (vertexType == eVT_VertexPT)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(VertPTf) * verticesCount, vertices, draw);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPTf), (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT,
                                      GL_FALSE, sizeof(VertPTf), (void*) sizeof(Vec3f));
            }
            else if (vertexType == eVT_VertexPNT)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNTf) * verticesCount, vertices, draw);

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
            else if (vertexType == eVT_VertexPNTBT)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(VertPNTBTf) * verticesCount, vertices, draw);

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
                FAIL(false, "Unknown vertex format [name: '%s']", mResourceName.get());
            }

            glGenBuffers(1, &mElementBufferObject);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferObject);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * indicesCount, indices, GL_STATIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            mIndicesCount = indicesCount;
            mVerticesCount = verticesCount;
            mPrimitiveMode = GLRenderDriver::TRIANGLES;
            mVertexType = vertexType;
            mIndexType = IRenderDriver::UNSIGNED_SHORT;
            mIndicesType = GLRenderDriver::UNSIGNED_SHORT;
        }

        void GLGPUBuffer::update(uint32 size, const void *data)
        {
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
            glBufferSubData(GL_ARRAY_BUFFER, 0 , size, data);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void GLGPUBuffer::draw(uint32 count,
                               IRenderDriver::PrimitiveType primitiveType,
                               IRenderDriver::DataType indicesType)
        {
            glBindVertexArray(mVertexArrayObject);
            glDrawElements(GLRenderDriver::getPrimitiveType(primitiveType),
                           count,
                           GLRenderDriver::getDataType(indicesType),
                           nullptr);
        }

        void GLGPUBuffer::draw()
        {
            glBindVertexArray(mVertexArrayObject);
            glDrawElements(mPrimitiveMode, mIndicesCount, mIndicesType, nullptr);
        }

        void GLGPUBuffer::draw(uint32 indicesCount)
        {
            FAIL(indicesCount <= mIndicesCount, "Indices to draw more than index buffer indices count");

            glBindVertexArray(mVertexArrayObject);
            glDrawElements(mPrimitiveMode, indicesCount, mIndicesType, nullptr);
        }

        IGPUBuffer::VertexType GLGPUBuffer::getVertexType()
        {
            return mVertexType;
        }

        uint32 GLGPUBuffer::getVertexCount()
        {
            return mVerticesCount;
        }

        uint32 GLGPUBuffer::getGPUMemoryUsage()
        {
            uint32 indexSize;

            switch (mIndexType)
            {
                case IRenderDriver::UNSIGNED_SHORT:
                    indexSize = sizeof(uint16);
                    break;

                case IRenderDriver::UNSIGNED_INT:
                    indexSize = sizeof(uint32);
                    break;

                default:
                    FAIL(false, "Unsupported index format [name: '%s']", mResourceName.get());
            }

            uint32 vertexSize;

            switch (mVertexType)
            {
                case VertexType::eVT_Vertex:
                    vertexSize = sizeof(Vertf);
                    break;

                case VertexType::eVT_VertexPN:
                    vertexSize = sizeof(VertPNf);
                    break;

                case VertexType::eVT_VertexPT:
                    vertexSize = sizeof(VertPTf);
                    break;

                case VertexType::eVT_VertexPNT:
                    vertexSize = sizeof(VertPNTf);
                    break;

                case VertexType::eVT_VertexPNTBT:
                    vertexSize = sizeof(VertPNTBTf);
                    break;

                default:
                    FAIL(false, "Unsupported vertex format [name: '%s']", mResourceName.get());
            }

            return mVerticesCount * vertexSize + mIndicesCount * indexSize;
        }

    } // namespace Resources

} // namespace Berserk
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLARRAYOBJECT_H
#define BERSERK_GLARRAYOBJECT_H

#include <RHI/RHIResources.h>
#include <GLVertexDeclaration.h>
#include <GLVertexBuffer.h>
#include <GLIndexBuffer.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLArrayObject : public RHIArrayObject {
    public:

        ~GLArrayObject() override {
            destroy();
        }

        bool create(const TArrayStatic<TPtrShared<RHIVertexBuffer>> &vertexData, const TPtrShared<RHIIndexBuffer> &indexData, const TPtrShared<RHIVertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET_VALUE(false, declaration.isNotNull(), "Null vertex declaration")
            BERSERK_COND_ERROR_RET_VALUE(false, vertexData.size() == declaration->getBuffersUses(), "Number of buffers in declarations and list differs");

            auto& GL_decl = (GLVertexDeclaration&) *declaration;
            auto& GL_buffers = GL_decl.getBufferDeclarations();
            auto& GL_vertices = GL_decl.getVertexDeclarations();

            glGenVertexArrays(1, &mObjectHandle);
            glBindVertexArray(mObjectHandle);

            for (uint32 i = 0; i < vertexData.size(); i++) {
                BERSERK_COND_ERROR_RET_VALUE(false, vertexData[i].isNotNull(), "Null vertex buffer at [%u] index", i);

                auto& GL_bufferDecl = GL_buffers[i];
                auto& GL_vertexBuffer = (GLVertexBuffer&) *vertexData[i];
                GLuint GL_attributeDivisor = (GL_bufferDecl.iterating == EVertexIterating::PerInstance ? 1 : 0);

                glBindBuffer(GL_ARRAY_BUFFER, GL_vertexBuffer.getBufferHandle());

                for (auto vertexIndex: GL_bufferDecl.vertices) {
                    auto& element = GL_vertices[vertexIndex];

                    uint32 count;
                    GLenum baseType;
                    GLDefinitions::getVertexElementType(element.type, baseType, count);

                    glEnableVertexAttribArray(element.location);
                    glVertexAttribDivisor(element.location, GL_attributeDivisor);
                    glVertexAttribPointer(element.location, count, baseType, GL_FALSE, element.stride, (void*)element.offset);

                    BERSERK_CATCH_OPENGL_ERRORS();
                }
            }

            if (indexData.isNotNull()) {
                auto& GL_indexBuffer = (GLIndexBuffer&) *indexData;
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_indexBuffer.getBufferHandle());
            }

            glBindVertexArray(GL_NONE);
            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

            BERSERK_CATCH_OPENGL_ERRORS();

            mVertexBuffers = vertexData;
            mIndexBuffer = indexData;
            mVertexDeclaration = declaration;

            return true;
        }

        void destroy() {
            if (mObjectHandle) {
                glDeleteVertexArrays(1, &mObjectHandle);
                mObjectHandle = 0;
                mVertexBuffers.clear();
                mIndexBuffer.free();

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        GLuint getObjectHandle() const {
            return mObjectHandle;
        }

    private:

        GLuint mObjectHandle = 0;

    };

}

#endif //BERSERK_GLARRAYOBJECT_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLVERTEXBUFFER_H
#define BERSERK_GLVERTEXBUFFER_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLVertexBuffer : public RHIVertexBuffer {
    public:

        ~GLVertexBuffer() override {
            destroy();
        }

        bool create(EMemoryType memoryType, uint32 size, const void *data) {
            BERSERK_COND_ERROR_RET_VALUE(false, size > 0, "Buffer size must be more than 0");

            mBufferMemoryType = memoryType;
            mBufferSize = size;

            auto glUsage = GLDefinitions::getMemoryType(memoryType);
            glGenBuffers(1, &mBufferHandle);
            glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle);
            glBufferData(GL_ARRAY_BUFFER, size, data, glUsage);
            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

            BERSERK_CATCH_OPENGL_ERRORS();

            return true;
        }

        void destroy() {
            if (mBufferHandle) {
                glDeleteBuffers(1, &mBufferHandle);
                mBufferHandle = 0;

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        void update(uint32 range, uint32 offset, const void *data) override {
            uint32 updateSection = offset + range;
            BERSERK_COND_ERROR_RET(updateSection <= mBufferSize, "Attempt to update out of buffer range");

            glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle);
            glBufferSubData(GL_ARRAY_BUFFER, offset, range, data);
            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        GLuint getBufferHandle() const {
            return mBufferHandle;
        }

    private:

        GLuint mBufferHandle = 0;

    };

}

#endif //BERSERK_GLVERTEXBUFFER_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLUNIFORMBUFFER_H
#define BERSERK_GLUNIFORMBUFFER_H

#include <RHI/RHIResources.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLUniformBuffer : public RHIUniformBuffer {
    public:

        ~GLUniformBuffer() override {
            destroy();
        }

        void create(EMemoryType memoryType, uint32 size) {
            BERSERK_COND_ERROR_FAIL(size > 0, "Buffer size must be more than 0");

            mBufferMemoryType = memoryType;
            mBufferSize = size;

            auto glUsage = GLDefinitions::getMemoryType(memoryType);
            glGenBuffers(1, &mBufferHandle);
            glBindBuffer(GL_UNIFORM_BUFFER, mBufferHandle);
            glBufferData(GL_UNIFORM_BUFFER, size, nullptr, glUsage);
            glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        void destroy() {
            glDeleteBuffers(1, &mBufferHandle);
            mBufferHandle = 0;

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        void update(uint32 range, uint32 offset, const uint8 *data) override {
            uint32 updateSection = offset + range;
            BERSERK_COND_ERROR_FAIL(updateSection <= mBufferSize, "Attempt to update out of buffer range");
            BERSERK_COND_ERROR_FAIL(mBufferMemoryType == EMemoryType::Dynamic, "Buffer is not dynamic");

            glBindBuffer(GL_UNIFORM_BUFFER, mBufferHandle);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, range, data);
            glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

            BERSERK_CATCH_OPENGL_ERRORS();
        }

        void bind(uint32 binding, uint32 range, uint32 offset) const {
            uint32 updateSection = offset + range;
            BERSERK_COND_ERROR_FAIL(updateSection <= mBufferSize, "Attempt to bind out of buffer range");

            glBindBufferRange(GL_UNIFORM_BUFFER, binding, mBufferHandle, offset, range);
            BERSERK_CATCH_OPENGL_ERRORS();
        }

        GLuint getBufferHandle() const {
            return mBufferHandle;
        }

    private:

        GLuint mBufferHandle = 0;

    };
}

#endif //BERSERK_GLUNIFORMBUFFER_H
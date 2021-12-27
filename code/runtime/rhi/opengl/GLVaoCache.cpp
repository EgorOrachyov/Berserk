/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <rhi/opengl/GLBuffer.hpp>
#include <rhi/opengl/GLVaoCache.hpp>

BRK_NS_BEGIN

GLVaoCache::GLVaoCache(uint32 releaseFrequency, uint32 timeToKeep)
    : mReleaseFrequency(releaseFrequency), mTimeToKeep(timeToKeep) {
}

GLVaoCache::~GLVaoCache() {
    for (auto &entry : mEntries) {
        auto &vao = entry.second;
        ReleaseVaoObject(vao);
    }
}

GLuint GLVaoCache::GetOrCreateVao(const GLVaoDescriptor &descriptor) {
    GLVaoKey key;
    key.Setup(descriptor);

    auto value = mEntries.find(key);

    // Found entry, return it and update frame used info
    if (value != mEntries.end()) {
        value->second.frameUsed = mCurrentFrame;
        return value->second.handle;
    }

    // Create new vao and place it into cache
    GLVaoValue vao{};
    CreateVaoObject(descriptor, vao);

    // Add entry into the cache
    mEntries.emplace(key, vao);

    return vao.handle;
}

void GLVaoCache::GC() {
    if (mCurrentFrame >= mLastRelease + mReleaseFrequency) {
        mLastRelease = mCurrentFrame;

        auto current = mEntries.cbegin();
        while (current != mEntries.cend()) {
            auto &e = *current;

            if (e.second.frameUsed + mTimeToKeep <= mCurrentFrame) {
                ReleaseVaoObject(e.second);
                current = mEntries.erase(current);
            } else
                ++current;
        }
    }

    mCurrentFrame += 1;
}

void GLVaoCache::CreateVaoObject(const GLVaoDescriptor &descriptor, GLVaoCache::GLVaoValue &vao) const {
    assert(descriptor.declaration);

    auto &elements = descriptor.declaration->GetElements();
    auto &buffers = descriptor.buffers;
    auto &indices = descriptor.indices;

    GLuint handle;

    glGenVertexArrays(1, &handle);
    BRK_GL_CATCH_ERR();

    glBindVertexArray(handle);
    BRK_GL_CATCH_ERR();

    for (uint32 bufferId = 0; bufferId < static_cast<uint32>(buffers.size()); bufferId++) {
        auto native = (GLVertexBuffer *) buffers[bufferId].Get();

        if (!native)
            break;

        glBindBuffer(GL_ARRAY_BUFFER, native->GetHandle());
        BRK_GL_CATCH_ERR();

        for (uint32 location = 0; location < static_cast<uint32>(elements.size()); location++) {
            auto &element = elements[location];
            if (element.buffer == bufferId) {
                GLenum baseType;
                GLuint components;
                GLDefs::GetVertexElementType(element.type, baseType, components);

                glEnableVertexAttribArray(location);
                BRK_GL_CATCH_ERR();

                glVertexAttribDivisor(location, element.frequency == RHIVertexFrequency::PerInstance ? 1 : 0);
                BRK_GL_CATCH_ERR();

                const uint8 *offset = nullptr;
                offset = offset + element.offset;

                glVertexAttribPointer(location, static_cast<GLint>(components), baseType, GL_FALSE, static_cast<GLsizei>(element.stride), offset);
                BRK_GL_CATCH_ERR();
            }
        }
    }

    if (indices) {
        auto native = (GLIndexBuffer *) indices.Get();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native->GetHandle());
        BRK_GL_CATCH_ERR();
    }

    glBindVertexArray(GL_NONE);
    BRK_GL_CATCH_ERR();

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    BRK_GL_CATCH_ERR();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
    BRK_GL_CATCH_ERR();

    vao.handle = handle;
    vao.frameUsed = mCurrentFrame;

    BRK_INFO("Create VAO hnd=" << vao.handle << " frame used=" << vao.frameUsed);
}

void GLVaoCache::ReleaseVaoObject(const GLVaoCache::GLVaoValue &vao) const {
    glDeleteVertexArrays(1, &vao.handle);
    BRK_GL_CATCH_ERR();
    BRK_INFO("Release VAO hnd=" << vao.handle << " frame used=" << vao.frameUsed);
}

BRK_NS_END
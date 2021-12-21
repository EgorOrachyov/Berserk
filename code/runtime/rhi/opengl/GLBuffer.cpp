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

BRK_NS_BEGIN

void GLBuffer::Initialize(GLenum type, uint32 size, RHIBufferUsage usage) {
    mType = type;

    glGenBuffers(1, &mHandle);
    BRK_GL_CATCH_ERR();

    glBindBuffer(mType, mHandle);
    BRK_GL_CATCH_ERR();

    glBufferData(mType, size, nullptr, GLDefs::GetBufferUsage(usage));
    BRK_GL_CATCH_ERR();

    glBindBuffer(mType, GL_NONE);
    BRK_GL_CATCH_ERR();
}

void GLBuffer::Finalize() {
    if (mHandle) {
        glDeleteBuffers(1, &mHandle);
        BRK_GL_CATCH_ERR();

        mHandle = 0;
    }
}

void GLBuffer::Update(uint32 size, uint32 byteOffset, uint32 byteSize, const void *memory) {
    assert(mHandle);
    assert(byteSize > 0);
    assert(byteOffset + byteSize <= size);
    assert(memory);

    glBindBuffer(mType, mHandle);
    BRK_GL_CATCH_ERR();

    glBufferSubData(mType, byteOffset, byteSize, memory);
    BRK_GL_CATCH_ERR();

    glBindBuffer(mType, GL_NONE);
    BRK_GL_CATCH_ERR();
}

GLVertexBuffer::GLVertexBuffer(const RHIBufferDesc &desc) {
    mBufferUsage = desc.bufferUsage;
    mSize = desc.size;
}

GLVertexBuffer::~GLVertexBuffer() {
    GLBuffer::Finalize();
}

void GLVertexBuffer::Initialize() {
    GLBuffer::Initialize(GL_ARRAY_BUFFER, mSize, mBufferUsage);
}

GLIndexBuffer::GLIndexBuffer(const RHIBufferDesc &desc) {
    mBufferUsage = desc.bufferUsage;
    mSize = desc.size;
}

GLIndexBuffer::~GLIndexBuffer() {
    GLBuffer::Finalize();
}

void GLIndexBuffer::Initialize() {
    GLBuffer::Initialize(GL_ELEMENT_ARRAY_BUFFER, mSize, mBufferUsage);
}

GLUniformBuffer::GLUniformBuffer(const RHIBufferDesc &desc) {
    mBufferUsage = desc.bufferUsage;
    mSize = desc.size;
}

GLUniformBuffer::~GLUniformBuffer() {
    GLBuffer::Finalize();
}

void GLUniformBuffer::Initialize() {
    GLBuffer::Initialize(GL_UNIFORM_BUFFER, mSize, mBufferUsage);
}

void GLUniformBuffer::Bind(uint32 location, uint32 offset, uint32 range) {
    assert(offset + range <= mSize);
    assert(range > 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, location, GetHandle(), offset, range);
    BRK_GL_CATCH_ERR();
}

BRK_NS_END

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

#ifndef BERSERK_GLBUFFER_HPP
#define BERSERK_GLBUFFER_HPP

#include <rhi/RHIBuffer.hpp>
#include <rhi/opengl/GLDefs.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLBuffer
 * @brief Base for GL buffer implementation
 */
class GLBuffer {
protected:
    void Initialize(GLenum type, uint32 size, RHIBufferUsage usage);
    void Finalize();
    void Update(uint32 size, uint32 byteOffset, uint32 byteSize, const void *memory);

public:
    GLuint GetHandle() const { return mHandle; }

private:
    GLuint mHandle = 0;
    GLenum mType = GL_INVALID_ENUM;
};

/**
 * @class GLVertexBuffer
 * @brief GL vertex buffer implementation
 */
class GLVertexBuffer final : public RHIVertexBuffer, public GLBuffer {
public:
    BRK_API explicit GLVertexBuffer(const RHIBufferDesc &desc);
    BRK_API ~GLVertexBuffer() override;

    BRK_API void Initialize();
    BRK_API void Update(uint32 byteOffset, uint32 byteSize, const void *memory) { GLBuffer::Update(GetSize(), byteOffset, byteSize, memory); };
};

/**
 * @class GLIndexBuffer
 * @brief GL index buffer implementation
 */
class GLIndexBuffer final : public RHIIndexBuffer, public GLBuffer {
public:
    BRK_API explicit GLIndexBuffer(const RHIBufferDesc &desc);
    BRK_API ~GLIndexBuffer() override;

    BRK_API void Initialize();
    BRK_API void Update(uint32 byteOffset, uint32 byteSize, const void *memory) { GLBuffer::Update(GetSize(), byteOffset, byteSize, memory); };
};

/**
 * @class GLUniformBuffer
 * @brief GL uniform buffer implementation
 */
class GLUniformBuffer final : public RHIUniformBuffer, public GLBuffer {
public:
    BRK_API explicit GLUniformBuffer(const RHIBufferDesc &desc);
    BRK_API ~GLUniformBuffer() override;

    BRK_API void Initialize();
    BRK_API void Update(uint32 byteOffset, uint32 byteSize, const void *memory) { GLBuffer::Update(GetSize(), byteOffset, byteSize, memory); };
    BRK_API void Bind(uint32 location, uint32 offset, uint32 range);
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLBUFFER_HPP

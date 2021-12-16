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

#ifndef BERSERK_RHIBUFFER_HPP
#define BERSERK_RHIBUFFER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIResource.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIBufferDesc
 * @brief RHI device buffer descriptor
 */
struct RHIBufferDesc {
    uint32 size;
    RHIBufferUsage bufferUsage;
};

/**
 * @class RHIBuffer
 * @brief RHI device buffer
 */
class RHIBuffer : public RHIResource {
public:
    ~RHIBuffer() override = default;

    /** @return Type of the buffer memory */
    RHIBufferUsage GetBufferUsage() const { return mBufferUsage; }

    /** @return Buffer total size in bytes */
    uint32 GetSize() const { return mSize; }

protected:
    RHIBufferUsage mBufferUsage;
    uint32 mSize;
};

/**
 * @class RHIVertexBuffer
 * @brief RHI vertex buffer
 */
class RHIVertexBuffer : public RHIBuffer {
public:
    BRK_API ~RHIVertexBuffer() override = default;
};

/**
 * @class RHIIndexBuffer
 * @brief RHI index buffer
 */
class RHIIndexBuffer : public RHIBuffer {
public:
    BRK_API ~RHIIndexBuffer() override = default;
};

/**
 * @class RHIUniformBuffer
 * @brief RHI uniform buffer
 */
class RHIUniformBuffer : public RHIBuffer {
public:
    BRK_API ~RHIUniformBuffer() override = default;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIBUFFER_HPP

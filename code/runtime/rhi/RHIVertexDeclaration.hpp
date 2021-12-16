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

#ifndef BERSERK_RHIVERTEXDECLARATION_HPP
#define BERSERK_RHIVERTEXDECLARATION_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIResource.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup rhi
 * @{
 */

/**
 * @class RHIVertexElement
 * @brief Describes a single vertex element in a vertex declaration.
 */
struct RHIVertexElement {
    uint32 offset;
    uint32 stride;
    uint8 buffer;
    RHIVertexElementType type;
    RHIVertexFrequency frequency;
};

/**
 * @class RHIVertexDeclarationDesc
 * @brief Describes input vertex layout
 */
using RHIVertexDeclarationDesc = std::vector<RHIVertexElement>;

/**
 * @class RHIVertexDeclaration
 * @brief Contains information about a vertex declaration.
 */
class RHIVertexDeclaration : public RHIResource {
public:
    BRK_API ~RHIVertexDeclaration() override = default;

    /** @return Elements declarations */
    const RHIVertexDeclarationDesc &GetElements() const { return mAttributes; }

protected:
    /** Attributes, used in the declaration */
    RHIVertexDeclarationDesc mAttributes;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RHIVERTEXDECLARATION_HPP

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

#ifndef BERSERK_MESHFORMATS_HPP
#define BERSERK_MESHFORMATS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/templates/Mask.hpp>
#include <rhi/RHIDefs.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

#include <unordered_map>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class MeshAttribute
 * @brief Available mesh vertex attributes
 */
enum class MeshAttribute : uint8 {
    /** float vec3 */
    Position = 0,
    /** float vec3 */
    Normal = 1,
    /** float vec3 */
    Tangent = 2,
    /** float vec3 */
    Color = 3,
    /** float vec2 */
    UV = 4,
    /** float vec2 */
    UV2 = 5,
    /** float vec4 */
    Weights = 6,
    /** int vec4 */
    Bones = 7
};

/**
 * @class MeshAttributeInfo
 * @brief Info about attribute
 */
struct MeshAttributeInfo {
    bool vertex = false;
    bool attribute = false;
    bool skinning = false;
    uint32 size = 0;
    RHIVertexElementType elementType = RHIVertexElementType::Unknown;
};

/**
 * @class MeshFormat
 * @brief Mask defining mesh format (composed from attributes)
 */
using MeshFormat = Mask<MeshAttribute, 8>;

/**
 * @class MeshFormats
 * @brief Manages vertex layouts for default engine mesh formats
 */
class MeshFormats final {
public:
    BRK_API MeshFormats() = default;
    BRK_API ~MeshFormats() = default;

    BRK_API Ref<RHIVertexDeclaration> GetDeclaration(MeshFormat target, MeshFormat format);
    BRK_API Ref<RHIVertexDeclaration> GetDeclaration(MeshFormat target);
    BRK_API uint32 GetAttributeOffset(MeshAttribute attribute, MeshFormat format);
    BRK_API void GetStride(MeshFormat format, uint32 &vertex, uint32 &attribute, uint32 &skinning);

private:
    std::unordered_map<String, Ref<RHIVertexDeclaration>> mCached;
};

/** @return Attribute info */
inline MeshAttributeInfo MeshGetAttributeInfo(MeshAttribute attribute) {
    MeshAttributeInfo info{};

    switch (attribute) {
        case MeshAttribute::Position:
        case MeshAttribute::Normal:
        case MeshAttribute::Tangent:
            info.vertex = true;
            info.size = 3 * sizeof(float);
            info.elementType = RHIVertexElementType::Float3;
            break;
        case MeshAttribute::Color:
            info.attribute = true;
            info.size = 3 * sizeof(float);
            info.elementType = RHIVertexElementType::Float3;
            break;
        case MeshAttribute::UV:
        case MeshAttribute::UV2:
            info.attribute = true;
            info.size = 2 * sizeof(float);
            info.elementType = RHIVertexElementType::Float2;
            break;
        case MeshAttribute::Weights:
            info.skinning = true;
            info.size = 4 * sizeof(float);
            info.elementType = RHIVertexElementType::Float4;
            break;
        case MeshAttribute::Bones:
            info.skinning = true;
            info.size = 4 * sizeof(int);
            info.elementType = RHIVertexElementType::Int4;
            break;
        default:
            break;
    }

    return info;
}

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_MESHFORMATS_HPP

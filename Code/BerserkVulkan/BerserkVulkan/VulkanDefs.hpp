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

#ifndef BERSERK_VULKANDEFS_HPP
#define BERSERK_VULKANDEFS_HPP

#include <vulkan/vulkan.h>
#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIPipelineState.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Color.hpp>
#include <BerserkCore/Image/PixelData.hpp>

#define BERSERK_LOG_VK BERSERK_TEXT("VK")

#define BERSERK_VK_LOG_INFO(...)                                                            \
        BERSERK_LOG_INFO(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_WARNING(...)                                                         \
        BERSERK_LOG_WARNING(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_ERROR(...)                                                           \
        BERSERK_LOG_ERROR(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_VK_LOG_FATAL(...)                                                           \
        BERSERK_LOG_FATAL(BERSERK_LOG_VK, __VA_ARGS__);

#define BERSERK_CHECK_ERROR(result)                                                         \
        if ((result) != VK_SUCCESS) { BERSERK_VK_LOG_ERROR("Function returned error"); }

#define BERSERK_VK_CHECK(function)                                                          \
        do { auto result = function; BERSERK_CHECK_ERROR(result) } while (false);           \

namespace Berserk {
    namespace RHI {

        class VulkanDefs {
        public:

            static VkShaderStageFlagBits GetShaderType(ShaderType type) {
                switch (type) {
                    case ShaderType::Vertex:
                        return VK_SHADER_STAGE_VERTEX_BIT;
                    case ShaderType::Fragment:
                        return VK_SHADER_STAGE_FRAGMENT_BIT;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported ShaderType");
                        return VK_SHADER_STAGE_ALL;
                }
            }

            static VkFormat GetVertexElementType(VertexElementType type) {
                switch (type) {
                    case VertexElementType::Float1:
                        return VK_FORMAT_R32_SFLOAT;
                    case VertexElementType::Float2:
                        return VK_FORMAT_R32G32_SFLOAT;
                    case VertexElementType::Float3:
                        return VK_FORMAT_R32G32B32_SFLOAT;
                    case VertexElementType::Float4:
                        return VK_FORMAT_R32G32B32A32_SFLOAT;
                    case VertexElementType::Int1:
                        return VK_FORMAT_R32_SINT;
                    case VertexElementType::Int2:
                        return VK_FORMAT_R32G32_SINT;
                    case VertexElementType::Int3:
                        return VK_FORMAT_R32G32B32_SINT;
                    case VertexElementType::Int4:
                        return VK_FORMAT_R32G32B32A32_SINT;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported VertexElementType");
                        return VK_FORMAT_MAX_ENUM;
                }
            }

            static VkVertexInputRate GetVertexFrequency(VertexFrequency frequency) {
                switch (frequency) {
                    case VertexFrequency::PerVertex:
                        return VK_VERTEX_INPUT_RATE_VERTEX;
                    case VertexFrequency::PerInstance:
                        return VK_VERTEX_INPUT_RATE_INSTANCE;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported VertexFrequency");
                        return VK_VERTEX_INPUT_RATE_MAX_ENUM;
                }
            }

            static VkPrimitiveTopology GetPrimitivesType(PrimitivesType type) {
                switch (type) {
                    case PrimitivesType::Triangles:
                        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                    case PrimitivesType::Lines:
                        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    case PrimitivesType::Points:
                        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported PrimitivesType");
                        return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
                }
            }

            static VkPolygonMode GetPolygonMode(PolygonMode mode) {
                switch (mode) {
                    case PolygonMode::Fill:
                        return VK_POLYGON_MODE_FILL;
                    case PolygonMode::Line:
                        return VK_POLYGON_MODE_LINE;
                    case PolygonMode::Point:
                        return VK_POLYGON_MODE_POINT;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported PolygonMode");
                        return VK_POLYGON_MODE_MAX_ENUM;
                }
            }

            static VkCullModeFlagBits GetPolygonCullMode(PolygonCullMode mode) {
                switch (mode) {
                    case PolygonCullMode::Disabled:
                        return VK_CULL_MODE_NONE;
                    case PolygonCullMode::Front:
                        return VK_CULL_MODE_FRONT_BIT;
                    case PolygonCullMode::Back:
                        return VK_CULL_MODE_BACK_BIT;
                    case PolygonCullMode::FrontAndBack:
                        return VK_CULL_MODE_FRONT_AND_BACK;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported PolygonCullMode");
                        return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
                }
            }

            static VkFrontFace GetPolygonFrontFace(PolygonFrontFace frontFace) {
                switch (frontFace) {
                    case PolygonFrontFace::Clockwise:
                        return VK_FRONT_FACE_CLOCKWISE;
                    case PolygonFrontFace::CounterClockwise:
                        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported PolygonFrontFace");
                        return VK_FRONT_FACE_MAX_ENUM;
                }
            }

            static VkBlendOp GetBlendOperation(BlendOperation operation) {
                switch (operation) {
                    case BlendOperation::Add:
                        return VK_BLEND_OP_ADD;
                    case BlendOperation::Subtract:
                        return VK_BLEND_OP_SUBTRACT;
                    case BlendOperation::ReverseSubtract:
                        return VK_BLEND_OP_REVERSE_SUBTRACT;
                    case BlendOperation::Min:
                        return VK_BLEND_OP_MIN;
                    case BlendOperation::Max:
                        return VK_BLEND_OP_MAX;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported BlendOperation");
                        return VK_BLEND_OP_MAX_ENUM;
                }
            }

            static VkBlendFactor GetBlendFactor(BlendFactor factor) {
                switch (factor) {
                    case BlendFactor::Zero:
                        return VK_BLEND_FACTOR_ZERO;
                    case BlendFactor::One:
                        return VK_BLEND_FACTOR_ONE;
                    case BlendFactor::SrcColor:
                        return VK_BLEND_FACTOR_SRC_COLOR;
                    case BlendFactor::OneMinusSrcColor:
                        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
                    case BlendFactor::DstColor:
                        return VK_BLEND_FACTOR_DST_COLOR;
                    case BlendFactor::OneMinusDstColor:
                        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
                    case BlendFactor::SrcAlpha:
                        return VK_BLEND_FACTOR_SRC_ALPHA;
                    case BlendFactor::OneMinusSrcAlpha:
                        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                    case BlendFactor::DstAlpha:
                        return VK_BLEND_FACTOR_DST_ALPHA;
                    case BlendFactor::OneMinusDstAlpha:
                        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
                    default:
                        BERSERK_VK_LOG_ERROR("Unsupported BlendFactor");
                        return VK_BLEND_FACTOR_MAX_ENUM;
                }
            }

            static bool IsSuitableForDepthStencil(TextureFormat format, bool& depth, bool& stencil) {
                switch (format) {
                    case TextureFormat::DEPTH32F:
                        depth = true;
                        stencil = false;
                        return true;
                    case TextureFormat::DEPTH32F_STENCIL8:
                    case TextureFormat::DEPTH24_STENCIL8:
                        depth = true;
                        stencil = true;
                        return true;
                    default:
                        depth = false;
                        stencil = false;
                        return false;
                }
            }

            static VkFormat GetTextureFormat(TextureFormat format) {
                switch (format) {
                    case TextureFormat::R8:
                        return VK_FORMAT_R8_UINT;
                    case TextureFormat::R8_SNORM:
                        return VK_FORMAT_R8_SNORM;
                    case TextureFormat::R16:
                        return VK_FORMAT_R16_UINT;
                    case TextureFormat::R16_SNORM:
                        return VK_FORMAT_R16_SNORM;
                    case TextureFormat::RG8:
                        return VK_FORMAT_R8G8_UINT;
                    case TextureFormat::RG8_SNORM:
                        return VK_FORMAT_R8G8_SNORM;
                    case TextureFormat::RG16:
                        return VK_FORMAT_R16G16_UINT;
                    case TextureFormat::RG16_SNORM:
                        return VK_FORMAT_R16G16_SNORM;
                    case TextureFormat::RGB8:
                        return VK_FORMAT_R8G8B8_UINT;
                    case TextureFormat::RGB8_SNORM:
                        return VK_FORMAT_R8G8B8_SNORM;
                    case TextureFormat::RGB16_SNORM:
                        return VK_FORMAT_R16G16B16_SNORM;
                    case TextureFormat::RGBA8:
                        return VK_FORMAT_R8G8B8A8_UINT;
                    case TextureFormat::RGBA8_SNORM:
                        return VK_FORMAT_R8G8B8A8_SNORM;
                    case TextureFormat::RGBA16:
                        return VK_FORMAT_R16G16B16A16_UINT;
                    case TextureFormat::SRGB8:
                        return VK_FORMAT_R8G8B8_SRGB;
                    case TextureFormat::SRGB8_ALPHA8:
                        return VK_FORMAT_R8G8B8A8_SRGB;
                    case TextureFormat::R16F:
                        return VK_FORMAT_R16_SFLOAT;
                    case TextureFormat::RG16F:
                        return VK_FORMAT_R16G16_SFLOAT;
                    case TextureFormat::RGB16F:
                        return VK_FORMAT_R16G16B16_SFLOAT;
                    case TextureFormat::RGBA16F:
                        return VK_FORMAT_R16G16B16A16_SFLOAT;
                    case TextureFormat::R32F:
                        return VK_FORMAT_R32_SFLOAT;
                    case TextureFormat::RG32F:
                        return VK_FORMAT_R32G32_SFLOAT;
                    case TextureFormat::RGB32F:
                        return VK_FORMAT_R32G32B32_SFLOAT;
                    case TextureFormat::RGBA32F:
                        return VK_FORMAT_R32G32B32A32_SFLOAT;
                    case TextureFormat::DEPTH32F:
                        return VK_FORMAT_D32_SFLOAT;
                    case TextureFormat::DEPTH32F_STENCIL8:
                        return VK_FORMAT_D32_SFLOAT_S8_UINT;
                    case TextureFormat::DEPTH24_STENCIL8:
                        return VK_FORMAT_D24_UNORM_S8_UINT;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported TextureFormat"));
                        return VK_FORMAT_MAX_ENUM;
                }
            }

            static bool DiscardsOnStart(RenderTargetOption option) {
                switch (option) {
                    case RenderTargetOption::DiscardStore:
                    case RenderTargetOption::DiscardDiscard:
                        return true;
                    default:
                        return false;
                }
            }

            static bool DiscardsOnEnd(RenderTargetOption option) {
                switch (option) {
                    case RenderTargetOption::ClearDiscard:
                    case RenderTargetOption::LoadDiscard:
                    case RenderTargetOption::DiscardDiscard:
                        return true;
                    default:
                        return false;
                }
            }

            static void GetLoadStoreOps(RenderTargetOption option, VkAttachmentLoadOp& load, VkAttachmentStoreOp& store) {
                switch (option) {
                    case RenderTargetOption::ClearStore:
                        load = VK_ATTACHMENT_LOAD_OP_CLEAR;
                        store = VK_ATTACHMENT_STORE_OP_STORE;
                        return;
                    case RenderTargetOption::ClearDiscard:
                        load = VK_ATTACHMENT_LOAD_OP_CLEAR;
                        store = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                        return;
                    case RenderTargetOption::LoadStore:
                        load = VK_ATTACHMENT_LOAD_OP_LOAD;
                        store = VK_ATTACHMENT_STORE_OP_STORE;
                        return;
                    case RenderTargetOption::LoadDiscard:
                        load = VK_ATTACHMENT_LOAD_OP_LOAD;
                        store = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                        return;
                    case RenderTargetOption::DiscardStore:
                        load = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                        store = VK_ATTACHMENT_STORE_OP_STORE;
                        return;
                    case RenderTargetOption::DiscardDiscard:
                        load = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                        store = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                        return;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported RenderTargetOption"));
                        return;
                }
            }

            static VkImageLayout GetDepthStencilSubpassLayout(bool depth, bool stencil) {
                if (depth || stencil) {
                    if (depth && stencil)
                        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    else if (depth)
                        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
                    else
                        return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
                }

                return VK_IMAGE_LAYOUT_UNDEFINED;
            }

            static VkCompareOp GetCompareFunction(CompareFunction function) {
                switch (function) {
                    case CompareFunction::Never:
                        return VK_COMPARE_OP_NEVER;
                    case CompareFunction::Less:
                        return VK_COMPARE_OP_LESS;
                    case CompareFunction::Equal:
                        return VK_COMPARE_OP_EQUAL;
                    case CompareFunction::LessEqual:
                        return VK_COMPARE_OP_LESS_OR_EQUAL;
                    case CompareFunction::Greater:
                        return VK_COMPARE_OP_GREATER;
                    case CompareFunction::GreaterEqual:
                        return VK_COMPARE_OP_GREATER_OR_EQUAL;
                    case CompareFunction::NotEqual:
                        return VK_COMPARE_OP_NOT_EQUAL;
                    case CompareFunction::Always:
                        return VK_COMPARE_OP_ALWAYS;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported CompareFunction"));
                        return VK_COMPARE_OP_MAX_ENUM;
                }
            }

            static VkStencilOp GetStencilOperation(Operation operation) {
                switch (operation) {
                    case Operation::Keep:
                        return VK_STENCIL_OP_KEEP;
                    case Operation::Zero:
                        return VK_STENCIL_OP_ZERO;
                    case Operation::Replace:
                        return VK_STENCIL_OP_REPLACE;
                    case Operation::Increment:
                        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
                    case Operation::Decrement:
                        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
                    case Operation::Invert:
                        return VK_STENCIL_OP_INVERT;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported Operation"));
                        return VK_STENCIL_OP_MAX_ENUM;
                }
            }

            static VkStencilOpState GetStencilOpState(PipelineState::DepthStencilState desc) {
                VkStencilOpState state{};
                state.reference = desc.referenceValue;
                state.compareMask = desc.compareMask;
                state.writeMask = desc.writeMask;
                state.compareOp = GetCompareFunction(desc.compareFunction);
                state.failOp = GetStencilOperation(desc.sfail);
                state.depthFailOp = GetStencilOperation(desc.dfail);
                state.passOp = GetStencilOperation(desc.dpass);

                return state;
            }

        };



    }
}

#endif //BERSERK_VULKANDEFS_HPP
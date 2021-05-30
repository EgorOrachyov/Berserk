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
#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Color.hpp>
#include <BerserkCore/Image/PixelData.hpp>
#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>

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

#ifndef VULKAN_VERSION
    #ifdef BERSERK_TARGET_MACOS
        #define VULKAN_VERSION VK_API_VERSION_1_1
    #else
        #define VULKAN_VERSION VK_API_VERSION_1_2
    #endif
#endif

namespace Berserk {
    namespace RHI {

        /** Structure for device initialization */
        struct VulkanDeviceInitInfo {
            String applicationName;
            String engineName;
            Array<String> requiredExtensions;
            SharedPtr<Window> primaryWindow;
            Function<VkResult(VkInstance,const SharedPtr<Window>&, VkSurfaceKHR&)> clientSurfaceFactory;
        };

        /** Info used to select physical device and configure swap chain */
        struct VulkanSwapChainSupportInfo {
            VkSurfaceCapabilitiesKHR capabilities{};
            Array<VkSurfaceFormatKHR> formats;
            Array<VkPresentModeKHR> presentModes;
            bool supportPresentation = false;
        };

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

            static VkImageType GetImageType(TextureType type) {
                switch (type) {
                    case TextureType::Texture2d:
                    case TextureType::Texture2dArray:
                    case TextureType::TextureCube:
                        return VK_IMAGE_TYPE_2D;
                    case TextureType::Texture3d:
                        return VK_IMAGE_TYPE_3D;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported TextureType"));
                        return VK_IMAGE_TYPE_MAX_ENUM;
                }
            }

            static VkImageViewType GetImageViewType(TextureType type) {
                switch (type) {
                    case TextureType::Texture2d:
                        return VK_IMAGE_VIEW_TYPE_2D;
                    case TextureType::Texture2dArray:
                        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
                    case TextureType::TextureCube:
                        return VK_IMAGE_VIEW_TYPE_CUBE;
                    case TextureType::Texture3d:
                        return VK_IMAGE_VIEW_TYPE_3D;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported TextureType"));
                        return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
                }
            }

            static bool CanCopyImage(TextureFormat format, PixelDataType type, PixelDataFormat pixelFormat) {
                switch (format) {
                    case TextureFormat::R8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::R8_SNORM:
                        return type == PixelDataType::BYTE &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::R16:
                        return type == PixelDataType::USHORT &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::R16_SNORM:
                        return type == PixelDataType::SHORT &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::RG8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RG8_SNORM:
                        return type == PixelDataType::BYTE &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RG16:
                        return type == PixelDataType::USHORT &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RG16_SNORM:
                        return type == PixelDataType::SHORT &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RGB8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::RGB8_SNORM:
                        return type == PixelDataType::BYTE &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::RGB16_SNORM:
                        return type == PixelDataType::SHORT &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::RGBA8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::RGBA8_SNORM:
                        return type == PixelDataType::BYTE &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::RGBA16:
                        return type == PixelDataType::USHORT &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::SRGB8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::SRGB8_ALPHA8:
                        return type == PixelDataType::UBYTE &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::R16F:
                        return type == PixelDataType::HALF &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::RG16F:
                        return type == PixelDataType::HALF &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RGB16F:
                        return type == PixelDataType::HALF &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::RGBA16F:
                        return type == PixelDataType::HALF &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::R32F:
                        return type == PixelDataType::FLOAT &&
                               pixelFormat == PixelDataFormat::R;
                    case TextureFormat::RG32F:
                        return type == PixelDataType::FLOAT &&
                               pixelFormat == PixelDataFormat::RG;
                    case TextureFormat::RGB32F:
                        return type == PixelDataType::FLOAT &&
                               pixelFormat == PixelDataFormat::RGB;
                    case TextureFormat::RGBA32F:
                        return type == PixelDataType::FLOAT &&
                               pixelFormat == PixelDataFormat::RGBA;
                    case TextureFormat::DEPTH32F:
                    case TextureFormat::DEPTH32F_STENCIL8:
                    case TextureFormat::DEPTH24_STENCIL8:
                        return false;
                    default:
                    BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported TextureFormat for copy"));
                        return VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
                }
            }

            static VkImageAspectFlags GetAspectFlags(TextureFormat format) {
                switch (format) {
                    case TextureFormat::R8:
                    case TextureFormat::R8_SNORM:
                    case TextureFormat::R16:
                    case TextureFormat::R16_SNORM:
                    case TextureFormat::RG8:
                    case TextureFormat::RG8_SNORM:
                    case TextureFormat::RG16:
                    case TextureFormat::RG16_SNORM:
                    case TextureFormat::RGB8:
                    case TextureFormat::RGB8_SNORM:
                    case TextureFormat::RGB16_SNORM:
                    case TextureFormat::RGBA8:
                    case TextureFormat::RGBA8_SNORM:
                    case TextureFormat::RGBA16:
                    case TextureFormat::SRGB8:
                    case TextureFormat::SRGB8_ALPHA8:
                    case TextureFormat::R16F:
                    case TextureFormat::RG16F:
                    case TextureFormat::RGB16F:
                    case TextureFormat::RGBA16F:
                    case TextureFormat::R32F:
                    case TextureFormat::RG32F:
                    case TextureFormat::RGB32F:
                    case TextureFormat::RGBA32F:
                        return VK_IMAGE_ASPECT_COLOR_BIT;
                    case TextureFormat::DEPTH32F:
                        return VK_IMAGE_ASPECT_DEPTH_BIT;
                    case TextureFormat::DEPTH32F_STENCIL8:
                    case TextureFormat::DEPTH24_STENCIL8:
                        return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported TextureFormat"));
                        return VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
                }
            }

            static VkFormat GetTextureFormat(TextureFormat format) {
                switch (format) {
                    case TextureFormat::R8:
                        return VK_FORMAT_R8_UNORM;
                    case TextureFormat::R8_SNORM:
                        return VK_FORMAT_R8_SNORM;
                    case TextureFormat::R16:
                        return VK_FORMAT_R16_UNORM;
                    case TextureFormat::R16_SNORM:
                        return VK_FORMAT_R16_SNORM;
                    case TextureFormat::RG8:
                        return VK_FORMAT_R8G8_UNORM;
                    case TextureFormat::RG8_SNORM:
                        return VK_FORMAT_R8G8_SNORM;
                    case TextureFormat::RG16:
                        return VK_FORMAT_R16G16_UNORM;
                    case TextureFormat::RG16_SNORM:
                        return VK_FORMAT_R16G16_SNORM;
                    case TextureFormat::RGB8:
                        return VK_FORMAT_R8G8B8_UNORM;
                    case TextureFormat::RGB8_SNORM:
                        return VK_FORMAT_R8G8B8_SNORM;
                    case TextureFormat::RGB16_SNORM:
                        return VK_FORMAT_R16G16B16_SNORM;
                    case TextureFormat::RGBA8:
                        return VK_FORMAT_R8G8B8A8_UNORM;
                    case TextureFormat::RGBA8_SNORM:
                        return VK_FORMAT_R8G8B8A8_SNORM;
                    case TextureFormat::RGBA16:
                        return VK_FORMAT_R16G16B16A16_UNORM;
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
                    case RenderTargetOption::ClearStore:
                    case RenderTargetOption::ClearDiscard:
                    case RenderTargetOption::DiscardStore:
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

            static VkIndexType GetIndexType(IndexType type) {
                switch (type) {
                    case IndexType::Uint32:
                        return VK_INDEX_TYPE_UINT32;
                    case IndexType::Uint16:
                        return VK_INDEX_TYPE_UINT16;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported IndexType"));
                        return VK_INDEX_TYPE_MAX_ENUM;
                }
            }

            static VkFilter GetMagFilter(SamplerMagFilter magFilter) {
                switch (magFilter) {
                    case SamplerMagFilter::Linear:
                        return VK_FILTER_LINEAR;
                    case SamplerMagFilter::Nearest:
                        return VK_FILTER_NEAREST;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMagFilter"));
                        return VK_FILTER_MAX_ENUM;
                }
            }

            static void GetMinFilter(SamplerMinFilter minFilter, VkFilter& filter, VkSamplerMipmapMode& mipmapMode) {
                switch (minFilter) {
                    case SamplerMinFilter::Nearest:
                        filter = VK_FILTER_NEAREST;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
                        break;
                    case SamplerMinFilter::Linear:
                        filter = VK_FILTER_LINEAR;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
                        break;
                    case SamplerMinFilter::NearestMipmapNearest:
                        filter = VK_FILTER_NEAREST;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
                        break;
                    case SamplerMinFilter::LinearMipmapNearest:
                        filter = VK_FILTER_LINEAR;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
                        break;
                    case SamplerMinFilter::NearestMipmapLinear:
                        filter = VK_FILTER_NEAREST;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
                        break;
                    case SamplerMinFilter::LinearMipmapLinear:
                        filter = VK_FILTER_LINEAR;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
                        break;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerMinFilter"));
                        filter = VK_FILTER_MAX_ENUM;
                        mipmapMode = VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
                        return;
                }
            }

            static VkSamplerAddressMode GetAddressMode(SamplerRepeatMode mode) {
                switch (mode) {
                    case SamplerRepeatMode::Repeat:
                        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
                    case SamplerRepeatMode::MirroredRepeat:
                        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
                    case SamplerRepeatMode::ClampToEdge:
                        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                    case SamplerRepeatMode::ClampToBorder:
                        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
                    case SamplerRepeatMode::MirrorClamToEdge:
                        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerRepeatMode"));
                        return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
                }
            }

            static VkBorderColor GetBorderColor(SamplerBorderColor color) {
                switch (color) {
                    case SamplerBorderColor::Black:
                        return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
                    case SamplerBorderColor::White:
                        return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
                    default:
                        BERSERK_VK_LOG_ERROR(BERSERK_TEXT("Unsupported SamplerBorderColor"));
                        return VK_BORDER_COLOR_MAX_ENUM;
                }
            }

        };



    }
}

#endif //BERSERK_VULKANDEFS_HPP
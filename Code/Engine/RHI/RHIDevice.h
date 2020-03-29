/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDEVICE_H
#define BERSERK_RHIDEVICE_H

#include <TArray.h>
#include <TPtrShared.h>
#include <Resources/Image.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /**
     * @brief Rendering device
     *
     * Generic rendering device class which wraps the functionality of
     * such lower API as OpenGL, Vulkan and DirectX and provides common
     * interface and object set for rendering operations.
     *
     * Complex objects are created by associated '*Desc' helper structures.
     * Follow this notation if you want to add device functionality.
     *
     * Object are referenced via shared pointer class, they could be safely
     * passed around and copied in context of the single thread. Also there is no
     * extra required destroy operations as well.
     *
     * @note Render device is created by Platform System object.
     * @note Render device could be accessed only from single Rendering Thread.
     * @note Render device does not provide window creation functionality (done by Platform System).
     */
    class RHIDevice {
    public:
        virtual ~RHIDevice() = default;

        virtual TPtrShared<RHIVertexBuffer> createVertexBuffer(uint32 size, EBufferType type) = 0;

        virtual TPtrShared<RHIIndexBuffer> createIndexBuffer(uint32 size, EBufferType type) = 0;

        virtual TPtrShared<RHIUniformBuffer> createUniformBuffer(uint32 size, EBufferType type) = 0;

        struct ShaderModuleDesc {
            EShaderType type;
            TArray<uint8> code;
        };

        struct ShaderDesc {
            EShaderLanguage language;
            TArray<ShaderModuleDesc> modules;
        };

        virtual TPtrShared<RHIShader> createShader(const ShaderDesc& shaderDesc) = 0;

        struct TextureDesc {
            ETextureType textureType;
            TArray<TPtrShared<Image>> images;
        };

        virtual TPtrShared<RHITexture> createTexture(const TextureDesc& textureDesc) = 0;

        struct SamplerDesc {
            ESamplerFilter min;
            ESamplerFilter mag;
            ESamplerFilter mipmapMode;
            ESamplerRepeatMode u;
            ESamplerRepeatMode v;
            ESamplerRepeatMode w;
            ESamplerBorderColor color;
            bool useAnisotropy = false;
        };

        virtual TPtrShared<RHISampler> createSampler(const SamplerDesc& samplerDesc) = 0;

        struct UniformTextureDesc {
            int32 binding;
            TPtrShared<RHITexture> texture;
            TPtrShared<RHISampler> sampler;
        };

        struct UniformBufferDesc {
            int32 binding;
            uint32 range;
            uint32 offset;
            TPtrShared<RHIUniformBuffer> buffer;
        };

        struct UniformSetDesc {
            TArray<UniformTextureDesc> textures;
            TArray<UniformBufferDesc> buffers;
        };

        virtual TPtrShared<RHIUniformSet> createUniformSet(const UniformSetDesc& uniformSetDesc) = 0;

        struct FramebufferDesc {
            TArray<TPtrShared<RHITexture>> colorAttachments;
            TPtrShared<RHITexture> depthStencilAttachment;
        };

        virtual TPtrShared<RHIFramebuffer> createFramebuffer(const FramebufferDesc& framebufferDesc) = 0;

        virtual const TArray<EImageFormat > &getSupportedTextureFormats() const = 0;

        virtual const TArray<EShaderLanguage> &getSupportedShaderLanguages() const = 0;

        // todo: resource creation and life cycle
        // todo: query device info
        // todo: device capabilities
    };

}

#endif //BERSERK_RHIDEVICE_H
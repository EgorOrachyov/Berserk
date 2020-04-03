/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLDEVICE_H
#define BERSERK_GLDEVICE_H

#include <Typedefs.h>
#include <TArray.h>
#include <TMap.h>
#include <RHI/RHIDevice.h>

namespace Berserk {

    using GLBindWindwoFunc = Function<void(ISystem::WINDOW_ID window)>;

    class GLDevice : public RHIDevice {
    public:

        GLDevice();

        ~GLDevice() override;

        TPtrShared<RHIVertexDeclaration> createVertexDeclaration(const RHIVertexDeclarationDesc &vertexDeclarationDesc) override;

        TPtrShared<RHIVertexBuffer> createVertexBuffer(uint32 size, EMemoryType type) override;

        TPtrShared<RHIIndexBuffer> createIndexBuffer(uint32 size, EMemoryType type) override;

        TPtrShared<RHIUniformBuffer> createUniformBuffer(uint32 size, EMemoryType type) override;

        TPtrShared<RHIShader> createShader(EShaderLanguage language, const RHIShaderDesc &modules) override;

        TPtrShared<RHITexture> createTexture2D(bool useMipMaps, const Image &image) override;

        TPtrShared<RHITexture> createTexture2D(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat format, bool useMipMaps) override;

        TPtrShared<RHISampler> createSampler(const RHISamplerDesc &samplerDesc) override;

        TPtrShared<RHIUniformSet> createUniformSet(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBufferDesc> &buffers) override;

        TPtrShared<RHIFramebuffer> createFramebuffer() override;

        TPtrShared<RHIDrawList> createDrawList() override;

        void beginRenderFrame() override;

        void endRenderFrame() override;

        ERenderDeviceType getDeviceType() const override;

        const TArray<EPixelFormat> &getSupportedTextureFormats() const override;

        const TArray<EShaderLanguage> &getSupportedShaderLanguages() const override;

        void setWindowBindFunction(ISystem::WINDOW_ID window, const GLBindWindwoFunc &function);

        GLBindWindwoFunc& getWindowBindFunction(ISystem::WINDOW_ID window);

    private:

        static TArray<EPixelFormat> mSupportedTextureFormats;
        static TArray<EShaderLanguage> mSupportedShaderLanguages;
        static TMap<ISystem::WINDOW_ID, GLBindWindwoFunc, THashRaw<ISystem::WINDOW_ID>> mWindowBindFunctions;

    };

}

#endif //BERSERK_GLDEVICE_H
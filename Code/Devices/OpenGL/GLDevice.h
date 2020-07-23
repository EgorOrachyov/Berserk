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
#include <Containers/TArray.h>
#include <Containers/TMap.h>
#include <RHI/RHIDevice.h>
#include <Console/AutoVariable.h>

namespace Berserk {

    class GLDevice : public RHIDevice {
    public:

        GLDevice();

        ~GLDevice() override;

        TPtrShared<RHIVertexDeclaration> createVertexDeclaration(const RHIVertexDeclarationDesc &vertexDeclarationDesc) override;

        TPtrShared <RHIVertexBuffer> createVertexBuffer(uint32 size, EMemoryType type, const void *data) override;

        TPtrShared <RHIIndexBuffer> createIndexBuffer(uint32 size, EMemoryType type, const void *data) override;

        TPtrShared<RHIUniformBuffer> createUniformBuffer(uint32 size, EMemoryType type, const void *data) override;

        TPtrShared<RHIArrayObject> createArrayObject(const TArrayStatic <TPtrShared<RHIVertexBuffer>> &vertexData, const TPtrShared <RHIIndexBuffer> &indexData, const TPtrShared <RHIVertexDeclaration> &declaration) override;

        TPtrShared<RHIShader> createShader(EShaderLanguage language, const RHIShaderDesc &modules) override;

        TPtrShared<RHIShader> createShader(EShaderLanguage language, const RHIShaderViewDesc &modules) override;

        TPtrShared<RHIShaderMetaData> createShaderMeta(const TPtrShared<RHIShader> &shader) override;

        TPtrShared<RHIShaderMetaData> createShaderMeta() override;

        TPtrShared<RHITexture> createTexture2D(EMemoryType memoryType, bool useMipMaps, const Image &image) override;

        TPtrShared<RHITexture> createTexture2D(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat format, bool useMipMaps) override;

        TPtrShared<RHISampler> createSampler(const RHISamplerDesc &samplerDesc) override;

        TPtrShared<RHIUniformSet> createUniformSet(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBlockDesc> &uniformBlocks) override;

        TPtrShared<RHIFramebuffer> createFramebuffer(const TArray<TPtrShared<RHITexture>> &colors, const TPtrShared<RHITexture> &depthStencil) override;

        TPtrShared<RHIGraphicsPipeline> createGraphicsPipeline(const RHIGraphicsPipelineDesc &pipelineDesc) override;

        TPtrShared<RHIDrawList> createDrawList() override;

        const TPtrShared<RHITexture> &getWhiteTexture() override;

        const TPtrShared<RHITexture> &getBlackTexture() override;

        void beginRenderFrame() override;

        void submitDrawList(const TPtrShared<RHIDrawList> &drawList) override;

        void endRenderFrame() override;

        ERenderDeviceType getDeviceType() const override;

        CString getDeviceName() const override;

        const TArray<EPixelFormat> &getSupportedTextureFormats() const override;

        const TArray<EShaderLanguage> &getSupportedShaderLanguages() const override;

    private:

        /** Possibly abort if error on GPU side */
        void checkToAbort(bool result, const char* message) const;

        TArray<EPixelFormat> mSupportedTextureFormats;
        TArray<EShaderLanguage> mSupportedShaderLanguages;

    };

}

#endif //BERSERK_GLDEVICE_H
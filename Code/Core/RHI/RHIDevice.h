/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIDEVICE_H
#define BERSERK_RHIDEVICE_H

#include <Image.h>
#include <Containers/TArray.h>
#include <TPtrShared.h>
#include <Containers/TArrayStatic.h>
#include <Platform/System.h>
#include <RHI/RHIDescs.h>
#include <RHI/RHIResources.h>
#include <RHI/RHIDefinitions.h>
#include <RHI/RHIShaderMetaData.h>

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

        RHIDevice();

        virtual ~RHIDevice() = default;

        virtual TPtrShared<RHIVertexDeclaration> createVertexDeclaration(const RHIVertexDeclarationDesc& vertexDeclarationDesc) = 0;

        /**
         * @brief Vertex buffer creation
         * 
         * Creates vertex buffer, which can be used as data input for vertex shader for
         * per vertex or instance elements setup.
         *
         * @param size Size of the buffer in bytes
         * @param type Type of the memory, how the memory wil be updated
         * @param data Pointer to the data to initilize buffer (null if initialization is not required)
         * @return Created vertex buffer reference (null if creation error occurs)   
         */
        virtual TPtrShared<RHIVertexBuffer> createVertexBuffer(uint32 size, EMemoryType type, const void *data) = 0;

        virtual TPtrShared<RHIIndexBuffer> createIndexBuffer(uint32 size, EMemoryType type, const void *data) = 0;

        virtual TPtrShared<RHIUniformBuffer> createUniformBuffer(uint32 size, EMemoryType type, const void *data) = 0;

        virtual TPtrShared<RHIArrayObject> createArrayObject(const TArrayStatic <TPtrShared<RHIVertexBuffer>> &vertexData, const TPtrShared <RHIIndexBuffer> &indexData, const TPtrShared <RHIVertexDeclaration> &declaration) = 0;

        virtual TPtrShared<RHIShader> createShader(EShaderLanguage language, const RHIShaderDesc &modules) = 0;

        virtual TPtrShared<RHIShader> createShader(EShaderLanguage language, const RHIShaderViewDesc &modules) = 0;

        /** 
         * @brief Shader meta-data creation
         *
         * Creates shader introspection info for provided valid compiled shader resource.
         *
         * @param shader Initialized compiled shader resource for introspection info
         * @return Created introspection info (null if some error occurs)
         */
        virtual TPtrShared<RHIShaderMetaData> createShaderMeta(const TPtrShared<RHIShader> &shader) = 0;

        /**
         * @brief Shader meta-data creation
         * 
         * Creates empty shader meda-data with no params and uniform data info primary
         * for further shader deserialization from disk.
         *
         * @return Created empty introspection container (for disk deserialization) 
         */
        virtual TPtrShared<RHIShaderMetaData> createShaderMeta() = 0;

        virtual TPtrShared<RHITexture> createTexture2D(EMemoryType memoryType, bool useMipMaps, const Image &image) = 0;

        virtual TPtrShared<RHITexture> createTexture2D(uint32 width, uint32 height, EMemoryType memoryType, EPixelFormat format, bool useMipMaps) = 0;

        virtual TPtrShared<RHISampler> createSampler(const RHISamplerDesc& samplerDesc) = 0;

        virtual TPtrShared<RHIUniformSet> createUniformSet(const TArray<RHIUniformTextureDesc> &textures, const TArray<RHIUniformBlockDesc> &uniformBlocks) = 0;

        virtual TPtrShared<RHIFramebuffer> createFramebuffer(const TArray<TPtrShared<RHITexture>> &colors, const TPtrShared<RHITexture> &depthStencil) = 0;

        virtual TPtrShared<RHIGraphicsPipeline> createGraphicsPipeline(const RHIGraphicsPipelineDesc &pipelineDesc) = 0;

        /**
         * @brief Draw list creation
         *
         * Creates empty draw list ready for draw commands capturing.
         * List stores shared references to the bounded RHI resources for safe memory operations,
         * therefore user might not store own references on client-side.
         *
         * @return Created draw list for commands (null if some error occurs)
         */
        virtual TPtrShared<RHIDrawList> createDrawList() = 0;

        /** @return Default universal RGBA8 white texture (read-only usage) */
        virtual const TPtrShared<RHITexture> &getWhiteTexture() = 0;

        /** @return Default universal RGBA8 black (read-only usage) */
        virtual const TPtrShared<RHITexture> &getBlackTexture() = 0;

        /**
         * @brief Begins rendering of the new frame
         *
         * Must be called when each new rendering frame is started.
         * This function call prepares device for rendering, possibly resets some rendering statistic.
         */
        virtual void beginRenderFrame() = 0;

        /**
         * @brief Submit draw list for rendering
         *
         * Submits completed draw list for the rendering on device
         * and waits until the rendering is finished.
         *
         * @param drawList Completed draw list to execute
         */
        virtual void submitDrawList(const TPtrShared<RHIDrawList> &drawList) = 0;

        /**
         * @brief Ends rendering of the frame
         *
         * Signals device that current rendering frame is finished.
         * Possibly collects some rendering stat or submits commands for rendering.
         */
        virtual void endRenderFrame() = 0;

        /** @return Type of the device */
        virtual ERenderDeviceType getDeviceType() const = 0;

        /** @return Pixel format of the depth stencil buffers in the platform windows */
        virtual EPixelFormat getWindowsDepthStencilPixelFormat() const = 0;

        /** @return Type of the device as string */
        virtual CString getDeviceName() const = 0;

        /** @return True if pixel format supported */
        virtual bool isFormatSupported(EPixelFormat pixelFormat) const = 0;

        /** @return Formats supported for creating textures */
        virtual const TArray<EPixelFormat> &getSupportedTextureFormats() const = 0;

        /** @return Shader languages accepted by this device */
        virtual const TArray<EShaderLanguage> &getSupportedShaderLanguages() const = 0;

        /** @return Globally accessible dynamic RHI device (initialized by target system) */
        static RHIDevice& getSingleton();

    private:
        /** Singleton reference */
        static RHIDevice* gDevice;
    };

}

#endif //BERSERK_RHIDEVICE_H
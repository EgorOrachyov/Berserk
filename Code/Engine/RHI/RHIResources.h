/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCES_H
#define BERSERK_RHIRESOURCES_H

#include <TMap.h>
#include <TArray.h>
#include <TEnumMask.h>
#include <TArrayStatic.h>
#include <TPtrShared.h>
#include <PixelFormat.h>
#include <Math/Color4.h>
#include <Math/Region2i.h>
#include <Resources/Image.h>
#include <Platform/ISystem.h>
#include <RHI/RHIDescs.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /** Base RHI resource class */
    class RHIResource {
    public:
        /** Virtual to enable usage of shared pointers */
        virtual ~RHIResource() = default;
    };

    class RHIShader : public RHIResource {
    public:
        ~RHIShader() override = default;

        /** @return Sub-stages of this gpu shader program */
        const TArrayStatic<EShaderType>& getShaderStages() const { return mShaderStages; }

    protected:
        /** Sub-stages of this gpu shader program */
        TArrayStatic<EShaderType> mShaderStages;
    };

    class RHIShaderIntrospection : public RHIResource {
    public:
        ~RHIShaderIntrospection() override = default;

        struct Attribute {
            CString name;
            int32 location;
            EShaderData data;
        };

        struct UniformBlock {
            /** Uniform block name */
            CString name;
            /** Block index */
            int32 binding;
            /** Total block size (to allocate uniform buffers) */
            int32 size;
            /** Stages of usage */
            TEnumMask<EShaderType> stageFlags;
            /** Names of the params in block */
            TArray<CString> params;
        };

        struct Parameter {
            /** Number of the elements in the array (if parameter array) */
            int32 array;
            /** Stride between array elements (or matrix columns) */
            int32 stride;
            /** Block index if param inside block */
            int32 block;
            /** Size of the param (for arrays size of the first element) */
            int32 size;
            /** Offset of the param inside uniform block */
            int32 offset;
            /** Uniform location (for params outside uniform blocks) */
            int32 location;
            /** Data type (basic for arrays) */
            EShaderData data;
            /** Stages of usage */
            TEnumMask<EShaderType> stageFlags;
        };

        static const uint32 MAX_UNIFORM_NAME_LENGTH = 512;

        static const uint32 MAX_UNIFORMS_COUNT = 512;

        static const int32 DEFAULT_UNIFORM_BLOCK_INDEX = -1;

        static const int32 DEFAULT_UNIFORM_OFFSET = -1;

        static const int32 DEFAULT_UNIFORM_LOCATION = -1;

        /** @return Program variables (samplers, and uniform blocks variables) */
        const TMap<CString,Parameter> &getParams() const { return mParams; }

        /** @return Program uniform blocks info */
        const TArray<UniformBlock> &getUniformBlocks() const { return mUniformBlocks; }

        /** @return Input attributes of the vertex shader (as main entry for graphics shader)*/
        const TArray<Attribute> &getVertexShaderAttributes() const { return mVertexShaderAttributes; }

        /** @return Input attributes of the fragment shader */
        const TArray<Attribute> &getFragmentShaderAttributes() const { return mFragmentShaderAttributes; }

    protected:
        /** Program variables (samplers, and uniform blocks variables) */
        TMap<CString,Parameter> mParams;

        /** Program uniform blocks info */
        TArray<UniformBlock> mUniformBlocks;

        /** Input attributes of the vertex shader (as main entry for graphics shader)*/
        TArray<Attribute> mVertexShaderAttributes;

        /** Input attributes of the fragment shader */
        TArray<Attribute> mFragmentShaderAttributes;
    };

    class RHIVertexDeclaration : public RHIResource {
    public:
        ~RHIVertexDeclaration() override = default;

        /** @return Total input locations in vertex shader */
        uint32 getLocationsUses() const { return mLocationsUses; }

        /** @return Total separate buffers usage */
        uint32 getBuffersUses() const { return mBuffersUses; }

        /** @return Instancing usage for this layout */
        bool getUsesInstancing() const { return mUsesInstancing; }

    protected:
        /** Total input locations in vertex shader */
        uint32 mLocationsUses = 0;

        /** Total separate buffers usage */
        uint32 mBuffersUses = 0;

        /** Instancing usage for this layout */
        bool mUsesInstancing = false;
    };

    class RHIVertexBuffer : public RHIResource {
    public:
        ~RHIVertexBuffer() override = default;

        /** Updates specified buffer range with offset on GPU */
        virtual void update(uint32 range, uint32 offset, const uint8 *data) = 0;

        /** @return Type of the buffer memory */
        EMemoryType getMemoryType() const { return mBufferMemoryType; }

        /** @return Buffer total size in bytes */
        uint32 getBufferSize() const { return mBufferSize; }

    protected:
        /** Type of the buffer memory */
        EMemoryType mBufferMemoryType;

        /** Buffer total size in bytes */
        uint32 mBufferSize = 0;
    };

    class RHIIndexBuffer : public RHIResource {
    public:
        ~RHIIndexBuffer() override = default;

        /** Updates specified buffer range with offset on GPU */
        virtual void update(uint32 range, uint32 offset, const uint8 *data) = 0;

        /** @return Type of the buffer memory */
        EMemoryType getMemoryType() const { return mBufferMemoryType; }

        /** @return Buffer total size in bytes */
        uint32 getBufferSize() const { return mBufferSize; }

    protected:
        /** Type of the buffer memory */
        EMemoryType mBufferMemoryType;

        /** Buffer total size in bytes */
        uint32 mBufferSize = 0;
    };

    class RHIArrayObject : public RHIResource {
    public:
        ~RHIArrayObject() override = default;

        /** @return Vertex buffer attached to the vertex array object */
        const TArrayStatic<TPtrShared<RHIVertexBuffer>> &getVertexBuffers() const { return mVertexBuffers; };

        /** @return Optional index buffer of this array object */
        const TPtrShared<RHIIndexBuffer> &getIndexBuffer() const { return mIndexBuffer; };

        /** @return True, if uses index buffer */
        bool getUsesIndexBuffer() const { return mIndexBuffer != nullptr; }

    protected:
        /** Vertex buffer attached to the vertex array object */
        TArrayStatic<TPtrShared<RHIVertexBuffer>> mVertexBuffers;

        /** Optional index buffer of this array object (indexing is primary technique for rendering) */
        TPtrShared<RHIIndexBuffer> mIndexBuffer;
    };

    class RHIUniformLayout : public RHIResource {
    public:
        ~RHIUniformLayout() override = default;
    };

    class RHIUniformBuffer : public RHIResource {
    public:
        ~RHIUniformBuffer() override = default;

        /** Updates specified buffer range with offset on GPU */
        virtual void update(uint32 range, uint32 offset, const uint8 *data) = 0;

        /** @return Type of the buffer memory */
        EMemoryType getMemoryType() const { return mBufferMemoryType; }

        /** @return Buffer total size in bytes */
        uint32 getBufferSize() const { return mBufferSize; }

    protected:
        /** Type of the buffer memory */
        EMemoryType mBufferMemoryType;

        /** Buffer total size in bytes */
        uint32 mBufferSize = 0;
    };

    class RHIUniformSet : public RHIResource {
    public:
        ~RHIUniformSet() override = default;

        /** @return Uniform textures for the set */
        const TArray<RHIUniformTextureDesc> &getTextures() const { return mTextures; }

        /** @return Uniform blocks for the set */
        const TArray<RHIUniformBlockDesc> &getUniformBlocks() const { return mUniformBlocks; }

    protected:
        /** Uniform textures for the set */
        TArray<RHIUniformTextureDesc> mTextures;

        /** Uniform blocks for the set */
        TArray<RHIUniformBlockDesc> mUniformBlocks;
    };

    class RHITexture : public RHIResource {
    public:
        ~RHITexture() override = default;

        /** @return Texture width in pixels */
        uint32 getWidth() const { return mWidth; }

        /** @return Texture height in pixels */
        uint32 getHeight() const { return mHeight; }

        /** @return Texture depth in pixels */
        uint32 getDepth() const { return mDepth; }

        /** @return Texture RHI type */
        ETextureType getTextureType() { return mTextureType; }

        /** @return Texture memory type (host visibility) */
        EMemoryType getMemoryType() { return mMemoryType; }

        /** @return Texture data format per pixels */
        EPixelFormat getPixelFormat() const { return mPixelFormat; }

    protected:
        /** Texture width in pixels */
        uint32 mWidth = 0;

        /** Texture height in pixels */
        uint32 mHeight = 0;

        /** Texture depth in pixels */
        uint32 mDepth = 1;

        /** Texture RHI type */
        ETextureType mTextureType;

        /** Texture memory type (host visibility) */
        EMemoryType mMemoryType;

        /** Texture data format per pixels */
        EPixelFormat mPixelFormat;
    };

    class RHISampler : public RHIResource {
    public:
        ~RHISampler() override = default;

        /** @return Sampler minification filter */
        ESamplerFilter getMinFilter() const { return mMin; }

        /** @return Sampler magnification filter */
        ESamplerFilter getMagFilter() const { return mMag; }

        /** @return Sampler filter along mip levels */
        ESamplerFilter getMipmapMode() const { return mMipmapMode; }

        /** @return Repeat mode along u axis (coordinate) */
        ESamplerRepeatMode getRepeatU() const { return mRepeatU; }

        /** @return Repeat mode along v axis (coordinate) */
        ESamplerRepeatMode getRepeatV() const { return mRepeatV; }

        /** @return Repeat mode along w axis (coordinate) */
        ESamplerRepeatMode getRepeatW() const { return mRepeatW; }

        /** @return Sampler border color for Clamp to border repeat mode */
        ESamplerBorderColor getBorderColor() const { return mBorderColor; }

        /** @return True if sample filtering uses mipmaps */
        bool getUsesMips() const { return mUseMips; }

        /** @return Min possible lod number */
        int32 getMinLodLevel() const { return mMinLod; }

        /** @return Max possible lod number */
        int32 getMaxLodLevel() const { return mMaxLod; }

    protected:

        /** Sampler minification filter */
        ESamplerFilter mMin;

        /** Sampler magnification filter */
        ESamplerFilter mMag;

        /** Sampler filter along mip levels */
        ESamplerFilter mMipmapMode;

        /** Repeat mode along u axis (coordinate) */
        ESamplerRepeatMode mRepeatU;

        /** Repeat mode along v axis (coordinate) */
        ESamplerRepeatMode mRepeatV;

        /** Repeat mode along w axis (coordinate) */
        ESamplerRepeatMode mRepeatW;

        /** Sampler border color for Clamp to border repeat mode */
        ESamplerBorderColor mBorderColor;

        /** True if sample filtering uses mipmaps */
        bool mUseMips;

        /** Min possible lod number */
        int32 mMinLod;

        /** Max possible lod number */
        int32 mMaxLod;

    };

    class RHIFramebuffer : public RHIResource {
    public:
        ~RHIFramebuffer() override = default;

        /**  Limit somehow color attachment count */
        static const uint32 MAX_COLOR_ATTACHMENTS = 16;

        /** Set clear color for desired attachment */
        void setClearColor(uint32 attachment, const Color4f &color) { mClearColors[attachment] = color; }

        /** Set clear value for depth buffer */
        void setDepthClear(float32 value) { mDepthClear = value; }

        /** Set clear value for stencil buffer */
        void setStencilClear(uint32 value) { mStencilClear = value; }

        /** Set framebuffer clear option */
        void setClearOption(EClearOption option, bool value) { mClearOptions.setFlag(option, value); }

        /** @return True, if this framebuffer uses depth stencil attachment*/
        bool useDepthStencil() const { return mDepthAttachment.isNotNull(); }

        /** @return Framebuffer width */
        uint32 getWidth() const { return mWidth; }

        /** @return Framebuffer height */
        uint32 getHeight() const { return mHeight; }

        /** @return Depth clear value */
        float32 getDepthClear() const { return mDepthClear; }

        /** @return Stencil clear value */
        int32 getStencilClear() const { return mStencilClear; }

        /** @return Options on framebuffer clear */
        const TEnumMask<EClearOption> &getClearOptions() const { return mClearOptions; }

        /** @return Clear colors for color attachments */
        const TArray<Color4f> &getClearColors() const { return mClearColors; }

        /** @return Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        const TArray<TPtrShared<RHITexture>> &getColorAttachments() const { return mColorAttachments; }

        /** @return Optional depth-stencil buffer (might be null) */
        const TPtrShared<RHITexture> &getDepthStencilAttachment() const { return mDepthAttachment; }

    protected:

        /** Framebuffer width */
        uint32 mWidth;

        /** Framebuffer height */
        uint32 mHeight;

        /** Depth buffer clear value */
        float32 mDepthClear = 1.0f;

        /** Stencil buffer clear value */
        int32 mStencilClear = 0;

        /** Options on framebuffer clear */
        TEnumMask<EClearOption> mClearOptions;

        /** Clear colors for color attachments */
        TArray<Color4f> mClearColors;

        /** Color write buffers of the framebuffer (from 0 .. to N - 1)*/
        TArray<TPtrShared<RHITexture>> mColorAttachments;

        /** Optional depth-stencil buffer (might be null) */
        TPtrShared<RHITexture> mDepthAttachment;
    };

    class RHIGraphicsPipeline : public RHIResource {
    public:
        ~RHIGraphicsPipeline() override = default;

        /** @return Pipeline shader program */
        TPtrShared<RHIShader> getShader() const { return mShader; }

        /** @return Primitives rendered through the pipeline */
        EPrimitivesType getPrimitivesType() const { return mPrimitivesType; }

        /** @return Mode of rasterized primitives on the screen */
        EPolygonMode getPolygonMode() const { return mPolygonMode; }

        /** @return Culling mode */
        EPolygonCullMode getPolygonCullMode() const { return mPolygonCullMode; }

        /** @return From facing to determine visible faces of primitives */
        EPolygonFrontFace getPolygonFrontFace() const { return mPolygonFrontFace; }

        /** @return Line with */
        float32 getLineWidth() const { return mLineWidth; }

        /** @return True if pipeline uses depth test */
        bool getDepthTest() const { return mDepthTest; }

        /** @return True if pipeline uses writes depth values to the depth buffer */
        bool getDepthWrite() const { return mDepthWrite; }

        /** @return Depth compare function between new value and value in the buffer */
        ECompareFunction getDepthCompareFunction() const { return mDepthCompare; }

        /** @return Blend state descriptor */
        const RHIBlendStateDesc &getBlendStateDesc() const { return mBlendStateDesc; }

        /** @return Stencil state description */
        const RHIStencilStateDesc &getStencilStateDesc() const { return mStencilStateDesc; }

        /** @return Pipeline framebuffer format descriptor */
        const RHIFramebufferFormatDesc &getFramebufferDesc() const { return mFramebufferDesc; }

    protected:
        /** Pipeline shader program */
        TPtrShared<RHIShader> mShader;

        /** Primitives rendered through the pipeline */
        EPrimitivesType mPrimitivesType;

        /** Mode of rasterized primitives on the screen */
        EPolygonMode mPolygonMode;

        /** Culling mode */
        EPolygonCullMode mPolygonCullMode;

        /** From facing to determine visible faces of primitives */
        EPolygonFrontFace mPolygonFrontFace;

        /** Line with */
        float32 mLineWidth;

        /** True if pipeline uses depth test */
        bool mDepthTest;

        /** True if pipeline uses writes depth values to the depth buffer */
        bool mDepthWrite;

        /** Depth compare function between new value and value in the buffer */
        ECompareFunction mDepthCompare;

        /** Blend state descriptor */
        RHIBlendStateDesc mBlendStateDesc; // todo: support blending

        /** Stencil state description */
        RHIStencilStateDesc mStencilStateDesc; // todo: support stencil test

        /** Pipeline framebuffer format descriptor */
        RHIFramebufferFormatDesc mFramebufferDesc;
    };

    class RHIDrawList : public RHIResource {
    public:
        ~RHIDrawList() override = default;

        /** Begin render list commands setup */
        virtual void begin() = 0;

        /** End render list commands setup */
        virtual void end() = 0;

        /** Bind platform window as a render target */
        virtual void bindWindow(ISystem::WINDOW_ID window, const Region2i &viewport, const Color4f &clearColor) = 0;

        /** Bind platform window as a render target */
        virtual void bindWindow(ISystem::WINDOW_ID window, const Region2i &viewport, const Color4f &clearColor, float32 clearDepth, int32 clearStencil) = 0;

        /** Bind frame buffer as render target for the list rendering */
        virtual void bindFramebuffer(const TPtrShared<RHIFramebuffer> &framebuffer, const Region2i &viewport) = 0;

        /** Bind graphics pipeline to configure rendering process */
        virtual void bindPipeline(const TPtrShared<RHIGraphicsPipeline> &pipeline) = 0;

        /** Bind uniform set to the pipeline */
        virtual void bindUniformSet(const TPtrShared<RHIUniformSet> &uniformSet) = 0;

        /** Bind array object as data to be rendered */
        virtual void bindArrayObject(const TPtrShared<RHIArrayObject> &object) = 0;

        /** Draw bound array object indexed */
        virtual void drawIndexed(EIndexType indexType, uint32 indexCount) = 0;

        /** Draw bound array object indexed with instancing and base offset for each indexed vertex */
        virtual void drawIndexedBaseOffset(EIndexType indexType, uint32 indexCount, uint32 baseOffset) = 0;

        /** Draw bound array object indexed with instancing */
        virtual void drawIndexedInstances(EIndexType indexType, uint32 indexCount, uint32 instanceCount) = 0;

        /** Draw bound array object with raw vertex count */
        virtual void draw(uint32 verticesCount) = 0;

        /** Draw bound array object instanced with raw vertex count */
        virtual void drawInstanced(uint32 verticesCount, uint32 instancesCount) = 0;

        /** Draw bound array object with raw vertex count with offset for the first vertex  */
        virtual void drawBaseOffset(uint32 verticesCount, uint32 baseOffset) = 0;

        /** @return Current state of the list */
        EDrawListState getDrawListState() const { return mListState; }

    protected:
        /** Current state of the list */
        EDrawListState mListState;
    };

    class RHITimeQuery : public RHIResource {
    public:
        ~RHITimeQuery() override = default;
    };

}

#endif //BERSERK_RHIRESOURCES_H

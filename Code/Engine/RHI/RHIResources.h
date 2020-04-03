/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIRESOURCES_H
#define BERSERK_RHIRESOURCES_H

#include <TArray.h>
#include <TPtrShared.h>
#include <TArrayStatic.h>
#include <Math/Color4f.h>
#include <Math/Region2i.h>
#include <Platform/ISystem.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    class RHIResource {
    public:
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
        virtual void update(uint32 range, uint32 offset, uint8* data) = 0;

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
        virtual void update(uint32 range, uint32 offset, uint8* data) = 0;

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
        virtual void update(uint32 range, uint32 offset, uint8* data) = 0;

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
    };

    class RHITexture : public RHIResource {
    public:
        ~RHITexture() override = default;
    };

    class RHISampler : public RHIResource {
    public:
        ~RHISampler() override = default;
    };

    class RHIFramebuffer : public RHIResource {
    public:
        ~RHIFramebuffer() override = default;
    };

    class RHIGraphicsPipeline : public RHIResource {
    public:
        ~RHIGraphicsPipeline() override = default;
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

        /** Bind frame buffer as render target for the list rendering */
        virtual void bindFramebuffer(const TPtrShared<RHIFramebuffer> &framebuffer) = 0;

        /** Bind graphics pipeline to configure rendering process */
        virtual void bindPipeline(const TPtrShared<RHIGraphicsPipeline> &pipeline) = 0;

        /** Bind uniform set to the pipeline */
        virtual void bindUniformSet(const TPtrShared<RHIUniformSet> &uniformSet) = 0;

        /** Bind array object as data to be rendered */
        virtual void bindArrayObject(const TPtrShared<RHIArrayObject> &object) = 0;

        /** Draw bound array object indexed */
        virtual void drawIndexed(EIndexType indexType, uint32 indexCount) = 0;

        /** Draw bound array object indexed with instancing */
        virtual void drawIndexedInstances(EIndexType indexType, uint32 indexCount, uint32 instanceCount) = 0;

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

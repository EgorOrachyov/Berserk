//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_GLDRIVER_H
#define BERSERK_GLDRIVER_H

#include <RHIDriver.h>

namespace Berserk
{

    /** OpenGL RHI Driver implementation and main access class */
    class GLDriver final : public RHIDriver
    {
    public:

         explicit GLDriver(IAllocator & allocator = Allocator::get());

        ~GLDriver() override;

        RHIVertexShaderRef createVertexShader(
                const char *code) override;

        RHIFragmentShaderRef createFragmentShader(
                const char *code) override;

        RHIGeometryShaderRef createGeometryShader(
                const char *code) override;

        RHIComputeShaderRef createComputeShader(
                const char *code) override;

        RHITessControlShaderRef createTessellationControlShader(
                const char *code) override;

        RHITessEvalShaderRef createTessellationEvaluationShader(
                const char *code) override;

        RHIShaderProgramRef createShaderProgram(
                RHIVertexShaderRef &vertexShader,
                RHIFragmentShaderRef &fragmentShader,
                const RHIShaderInitializer &initializer) override;

        RHIVertexBufferRef createVertexBuffer(
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage) override;

        RHIIndexBufferRef createIndexBuffer(
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage,
                EIndexType indexType,
                uint32 indexCount) override;

        RHIGeometryBufferRef createGeometryBuffer(
                const RHIVertexBufferRef &vertexBuffer,
                const RHIIndexBufferRef &indexBuffer,
                EDataLayout layout,
                EPrimitiveType primitiveType) override;

        RHITexture2DRef createTexture(
                uint32 width,
                uint32 height,
                EStorageFormat storageFormat,
                EPixelFormat pixelFormat,
                EDataType dataType,
                bool genMipMaps) override;

        RHITexture2DRef createTexture(
                uint32 width,
                uint32 height,
                EStorageFormat storageFormat) override;

        RHITextureCubeRef createTexture(
                uint32 size,
                EStorageFormat storageFormat) override;

        RHIFrameBufferRef createFrameBuffer(
                uint32 width,
                uint32 height,
                EStorageFormat format) override;

        RHIFrameBufferRef createFrameBuffer(
                const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthAttachment) override;

        RHIFrameBufferRef createFrameBuffer_DepthStencil(
                const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthStencilAttachment) override;

        RHISamplerRef createSampler(
                ESamplerFilter filterMin,
                ESamplerFilter filterMag,
                ESamplerWrapMode wrapMode) override;

        RHIUniformBufferRef createUniformBuffer(
                uint32 bindingPoint,
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage) override;

        RHIDepthTestStateRef createDepthState(
                bool writeMask,
                ECompareFunc compareFunc) override;

        RHIBlendStateRef createBlendState(
                EBlendFunc source,
                EBlendFunc destination) override;

        RHIStencilTestStateRef createStencilState(
                uint32 mask,
                uint32 clear,
                ECompareFunc compare,
                uint32 value,
                uint32 read,
                EStencilOperation st_fail,
                EStencilOperation dt_fail,
                EStencilOperation dt_pass) override;

        RHIFaceCullingStateRef createFaceCullingState(
                EFaceCulling face,
                ERasterCullMode order) override;

        void setViewport(const ViewPort &view) override;

        void swapBuffers() override;

        void bindDefaultFrameBuffer() override;

        void setFillMode(ERasterFillMode fillMode) override;

        void disableDepthTest() override;

        void disableStencilTest() override;

        void disableBlending() override;

        void disableFaceCulling() override;

        const String &getDriverName() override;

        const String &getVendorName() override;

        const String &getShadingLanguageName() override;

    private:

        static void generateArrayWithCode(TArray<char> &code, const char *source);

        static void addShaderProgramInfo(void* programPtr, const RHIShaderInitializer& initializer);

    private:

        /** Allocate all the GL driver resources */
        IAllocator& mAllocator;

        /** Shader programs maps' uniform pool */
        PoolAllocator mUniformMapPool;

        /** Driver info */
        String mDriverName;
        String mVendorName;
        String mShadingLanguageName;

    };

} // namespace Berserk

#endif //BERSERK_GLDRIVER_H
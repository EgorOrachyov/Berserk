//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIDRIVER_H
#define BERSERK_RHIDRIVER_H

#include <RHI/RHIResources.h>
#include <Rendering/ShaderInitializer.h>
#include <Math/Rect.h>

namespace Berserk
{

    /** RHI driver for low-level rendering API access (OpenGL, DirectX, Vulkan) */
    class GRAPHICS_API RHIDriver
    {
    public:

        virtual ~RHIDriver() = default;

        /** Creates RHI support utils */
        virtual RHISupportRef createSupport() = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHIVertexShaderRef createVertexShader(
                const char* code) = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHIFragmentShaderRef createFragmentShader(
                const char* code) = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHIGeometryShaderRef createGeometryShader(
                const char* code) = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHIComputeShaderRef createComputeShader(
                const char* code) = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHITessControlShaderRef createTessellationControlShader(
                const char* code) = 0;

        /** Creates vertex shader from source code [with '\0' terminate symbol] */
        virtual RHITessEvalShaderRef createTessellationEvaluationShader(
                const char* code) = 0;

        /**
         * Creates linked shader program
         * @param vertexShader Vertex shader processor
         * @param fragmentShader Fragment shader processor
         * @param initializer Shader program additional uniform/subroutine info
         * @return Initialized and linked shader program ref
         */
        virtual RHIShaderProgramRef createShaderProgram(
                RHIVertexShaderRef& vertexShader,
                RHIFragmentShaderRef& fragmentShader,
                const ShaderInitializer& initializer) = 0;

        /**
         * Creates vertex buffer
         * @param size Buffer size in bytes
         * @param data Pointer to buffer data
         * @param bufferUsage Usage: dynamic/static for fast update
         * @return Vertex buffer ref
         */
        virtual RHIVertexBufferRef createVertexBuffer(
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage) = 0;

        /**
         * Creates index buffer for indexing vertices data
         * @param size Buffer size in bytes
         * @param data Pointer to buffer data
         * @param bufferUsage Usage: dynamic/static for fast update
         * @param indexType Type of indices in the buffer
         * @param indexCount Number of indices in buffer
         * @return Index buffer ref
         */
        virtual RHIIndexBufferRef createIndexBuffer(
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage,
                EIndexType indexType) = 0;

        /**
         * Creates simple indexed vertex draw buffer
         * @param vertexBuffer Vertices data
         * @param indexBuffer Indices data for proper vertex order
         * @param layout Data type of vertices
         * @param primitiveType Type of primitives to render
         * @return Geometry buffer
         */
        virtual RHIGeometryBufferRef createGeometryBuffer(
                const RHIVertexBufferRef &vertexBuffer,
                const RHIIndexBufferRef &indexBuffer,
                EDataLayout layout,
                EPrimitiveType primitiveType) = 0;

        virtual RHITexture2DRef createTexture(
                uint32 width,
                uint32 height,
                EStorageFormat storageFormat,
                EPixelFormat pixelFormat,
                EDataType dataType,
                uint8* data,
                bool genMipMaps) = 0;

        virtual RHITexture2DRef createTexture(
                uint32 width,
                uint32 height,
                EStorageFormat storageFormat) = 0;

        virtual RHITextureCubeRef createTexture(
                uint32 size,
                EStorageFormat storageFormat) = 0;

        virtual RHIFrameBufferRef createFrameBuffer(
                uint32 width,
                uint32 height,
                EStorageFormat format) = 0;

        virtual RHIFrameBufferRef createFrameBuffer(
                const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthAttachment) = 0;

        virtual RHIFrameBufferRef createFrameBuffer_DepthStencil(
                const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthStencilAttachment) = 0;

        virtual RHISamplerRef createSampler(
                ESamplerFilter filterMin,
                ESamplerFilter filterMag,
                ESamplerWrapMode wrapMode) = 0;

        virtual RHIUniformBufferRef createUniformBuffer(
                uint32 bindingPoint,
                uint32 size,
                const uint8 *data,
                EBufferUsage bufferUsage) = 0;

        /**
         * Create depth testing state, specify writing mask and
         * compare function to pass test.
         *
         * @param writeMask Writing mask (True to write, otherwise False)
         * @param compareFunc Compare value of fragment with value in the buffer
         */
        virtual RHIDepthTestStateRef createDepthState(
                bool writeMask,
                ECompareFunc compareFunc) = 0;

        /**
         * Create blending state, specify blending equation source
         * and destination coefficients.
         *
         * @param source Blending equation source coefficient
         * @param destination Blending equation destination (buffer value) coefficient
         *
         * @note Blending equation:
         *       result = source * s_color + destination * d_color
         */
        virtual RHIBlendStateRef createBlendState(
                EBlendFunc source,
                EBlendFunc destination) = 0;

        /**
         * Create stencil state, setup test param, writing mask,
         * compare function, and stencil test pass / fail options.
         *
         * @param mask Writing mast (8 bit unsigned integer)
         * @param clear Clear value for buffer (8 bit unsigned integer)
         * @param compare (Compare function for the test (Compare data from buffer and value)
         * @param value (Right compare value for compare function)
         * @param read (Mask to AND value and data stored in the buffer)
         * @param st_fail What to do when stencil test failed
         * @param dt_fail What to do when stencil test pass, but depth test failed
         * @param dt_pass What to do when stencil and depth test passed
         */
        virtual RHIStencilTestStateRef createStencilState(
                uint32 mask,
                uint32 clear,
                ECompareFunc compare,
                uint32 value,
                uint32 read,
                EStencilOperation st_fail,
                EStencilOperation dt_fail,
                EStencilOperation dt_pass) = 0;

        /**
         * Create face culling state, specify faces to cull, and
         * winding order for front face.
         *
         * @param face Which face cull
         * @param order Winding order to define FRONT faces
         */
        virtual RHIFaceCullingStateRef createFaceCullingState(
                EFaceCulling face,
                ERasterCullMode order) = 0;

        /** Set clear color for color buffer */
        virtual void setClearColor(const Vec4f &color) = 0;

        /** Clears current active render target color buffer */
        virtual void clearColorBuffer() = 0;

        /** Set value to clear depth buffer */
        virtual void setDepthClearValue(float value) = 0;

        /** Clear depth buffer with set value */
        virtual void clearDepthBuffer() = 0;

        /** Specify screen viewport */
        virtual void setViewport(const Rect& view) = 0;

        /** Double buffering swap buffers */
        virtual void swapBuffers() = 0;

        /** Bind display frame buffer */
        virtual void bindDefaultFrameBuffer() = 0;

        /** Set primitives fill mode */
        virtual void setFillMode(ERasterFillMode fillMode) = 0;

        /** Disable depth testing (previously enabled state) */
        virtual void disableDepthTest() = 0;

        /** Disable stencil test (previously enabled state) */
        virtual void disableStencilTest() = 0;

        /** Disable blending (previously enabled state) */
        virtual void disableBlending() = 0;

        /** Disable face culling (previously enabled state) */
        virtual void disableFaceCulling() = 0;

        /** @return Hardware info */
        virtual const String& getDriverName() = 0;

        /** @return Hardware info */
        virtual const String& getVendorName() = 0;

        /** @return Hardware info */
        virtual const String& getShadingLanguageName() = 0;

    };

    typedef TSharedPtr<RHIDriver> RHIDriverRef;

} // namespace Berserk

#endif //BERSERK_RHIDRIVER_H
//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIDRIVER_H
#define BERSERK_RHIDRIVER_H

#include <RHIResources.h>
#include <RHIShaderInitializer.h>

namespace Berserk
{

    class GRAPHICS_API RHIDriver
    {
    public:

        virtual ~RHIDriver() = default;

        virtual RHIVertexShaderRef createVertexShader(const char* code) = 0;

        virtual RHIFragmentShaderRef createFragmentShader(const char* code) = 0;

        virtual RHIGeometryShaderRef createGeometryShader(const char* code) = 0;

        virtual RHIComputeShaderRef createComputeShader(const char* code) = 0;

        virtual RHITessControlShaderRef createTessellationControlShader(const char* code) = 0;

        virtual RHITessEvalShaderRef createTessellationEvaluationShader(const char* code) = 0;

        virtual RHIShaderProgramRef createShaderProgram(const RHIVertexShaderRef& vertexShader,
                const RHIFragmentShaderRef& fragmentShader, const RHIShaderInitializer& initializer) = 0;

        virtual RHIVertexBufferRef createVertexBuffer(uint32 size, const uint8* data) = 0;

        virtual RHIIndexBufferRef createIndexBuffer(EIndexType type, uint32 size, const uint8* data) = 0;

        virtual RHIGeometryBufferRef createGeometryBuffer(const RHIVertexBufferRef& vertexBuffer,
                const RHIIndexBufferRef& indexBuffer,
                EDataLayout layout) = 0;

        virtual RHITexture2DRef createTexture(uint32 width, uint32 height,
                EStorageFormat storageFormat, EPixelFormat pixelFormat, EDataType dataType, bool genMipMaps) = 0;

        virtual RHITexture2DRef createTexture(uint32 width, uint32 height,
                EStorageFormat storageFormat) = 0;

        virtual RHITextureCubeRef createTexture(uint32 size, EStorageFormat storageFormat) = 0;

        virtual RHIFrameBufferRef createFrameBuffer() = 0;

        virtual RHIFrameBufferRef createFrameBuffer(const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthAttachment) = 0;

        virtual RHIFrameBufferRef createFrameBuffer_DepthStencil(const TArray<RHITextureRef> &colorAttachments,
                const RHITexture2DRef &depthStencilAttachment) = 0;

        virtual RHISamplerRef createSampler(ESamplerFilter filterMin, ESamplerFilter filterMag,
                ESamplerWrapMode wrapMode) = 0;

        virtual RHIUniformBufferRef createUniformBuffer(uint32 bindingPoint, uint32 size, const uint8* data) = 0;

        /**
         * Create depth testing state, specify writing mask and
         * compare function to pass test.
         *
         * @param writeMask Writing mask (True to write, otherwise False)
         * @param compareFunc Compare value of fragment with value in the buffer
         */
        virtual RHIDepthTestStateRef createDepthState(bool writeMask, ECompareFunc compareFunc) = 0;

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
        virtual RHIBlendStateRef createBlendState(EBlendFunc source, EBlendFunc destination) = 0;

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
        virtual RHIStencilTestStateRef createStencilState(uint32 mask, uint32 clear,
                ECompareFunc compare, uint32 value, uint32 read,
                EStencilOperation st_fail, EStencilOperation dt_fail, EStencilOperation dt_pass) = 0;

        /**
         * Create face culling state, specify faces to cull, and
         * winding order for front face.
         *
         * @param face Which face cull
         * @param order Winding order to define FRONT faces
         */
        virtual RHIFaceCullingStateRef createFaceCullingState(EFaceCulling face, ERasterCullMode order) = 0;

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

} // namespace Berserk

#endif //BERSERK_RHIDRIVER_H
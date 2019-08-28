//
// Created by Egor Orachyov on 2019-07-15.
//

#include "GLDriver.h"
#include <GLResources.h>
#include <GLEnums.h>

namespace Berserk
{

    GLDriver::GLDriver(Berserk::IAllocator &allocator)
        : mAllocator(allocator)
    {
        // Setup glew - OpenGL interface provider
        // Note: that no explicit close is needed

        if (glewInit() != GLEW_OK)
        {
            DEBUG_LOG_ERROR("GLDriver: cannot initialize glew library");
            return;
        }

        // Get info from render API
        getHardwareInfo();
    }

    GLDriver::~GLDriver()
    {
        DEBUG_LOG_DISPLAY("GLDriver: destroy");
    }

    RHISupportRef GLDriver::createSupport()
    {
        auto support = mAllocator.mem_new<GLSupport>();
        return RHISupportRef(support, &mAllocator);
    }

    RHIVertexShaderRef GLDriver::createVertexShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLVertexShader>(code);

        if (shader->isCompiled()) return RHIVertexShaderRef(shader, &mAllocator);

        return RHIVertexShaderRef();
    }

    RHIFragmentShaderRef GLDriver::createFragmentShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLFragmentShader>(code);

        if (shader->isCompiled()) return RHIFragmentShaderRef(shader, &mAllocator);

        return RHIFragmentShaderRef();
    }

    RHIGeometryShaderRef GLDriver::createGeometryShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLGeometryShader>(code);

        if (shader->isCompiled()) return RHIGeometryShaderRef(shader, &mAllocator);

        return RHIGeometryShaderRef();
    }

    RHIComputeShaderRef GLDriver::createComputeShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLComputeShader>(code);

        if (shader->isCompiled()) return RHIComputeShaderRef(shader, &mAllocator);

        return RHIComputeShaderRef();
    }

    RHITessControlShaderRef GLDriver::createTessellationControlShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLTessControlShader>(code);

        if (shader->isCompiled()) return RHITessControlShaderRef(shader, &mAllocator);

        return RHITessControlShaderRef();
    }

    RHITessEvalShaderRef GLDriver::createTessellationEvaluationShader(const char *code)
    {
        auto shader = mAllocator.mem_new<GLTessEvalShader>(code);

        if (shader->isCompiled()) return RHITessEvalShaderRef(shader, &mAllocator);

        return RHITessEvalShaderRef();
    }

    RHIShaderProgramRef GLDriver::createShaderProgram(
            const RHIVertexShaderRef &vertexShader,
            const RHIFragmentShaderRef &fragmentShader)
    {
        auto program = mAllocator.mem_new<GLShaderProgramVF>(
                vertexShader,
                fragmentShader);

        if (program->isLinked()) return RHIShaderProgramRef(program, &mAllocator);

        return RHIShaderProgramRef();
    }

    RHIVertexBufferRef GLDriver::createVertexBuffer(uint32 size, const uint8 *data, EBufferUsage bufferUsage)
    {
        auto buffer = mAllocator.mem_new<GLVertexBuffer>(
                GLEnums::BufferUsage(bufferUsage),
                size,
                data);

        return RHIVertexBufferRef(buffer, &mAllocator);
    }

    RHIIndexBufferRef
    GLDriver::createIndexBuffer(uint32 size, const uint8 *data, EBufferUsage bufferUsage, EIndexType indexType)
    {
        auto buffer = mAllocator.mem_new<GLIndexBuffer>(
                GLEnums::BufferUsage(bufferUsage),
                size,
                data,
                GLEnums::IndexType(indexType));

        return RHIIndexBufferRef(buffer, &mAllocator);
    }

    RHIGeometryBufferRef GLDriver::createGeometryBuffer(
            const RHIVertexBufferRef &vertexBuffer,
            const RHIIndexBufferRef &indexBuffer,
            EDataLayout layout,
            EPrimitiveType primitiveType)
    {
        auto gl_indexBuffer = (GLIndexBuffer*) indexBuffer.pointer();

        GLenum indicesType = gl_indexBuffer->getIndicesType();
        GLenum gl_primitiveType = GLEnums::PrimitiveType(primitiveType);

        auto buffer = mAllocator.mem_new<GLIndexedGeometryBuffer>(
                vertexBuffer,
                indexBuffer,
                layout,
                primitiveType,
                gl_primitiveType,
                indicesType);

        return RHIGeometryBufferRef(buffer, &mAllocator);
    }

    RHITexture2DRef GLDriver::createTexture(
            uint32 width,
            uint32 height,
            EStorageFormat storageFormat,
            EPixelFormat pixelFormat,
            EDataType dataType,
            uint8* data,
            bool genMipMaps)
    {
        auto gl_storageFormat = GLEnums::StorageFormat(storageFormat);
        auto gl_pixelFormat = GLEnums::PixelFormat(pixelFormat);
        auto gl_dataType = GLEnums::DataType(dataType);

        auto texture = mAllocator.mem_new<GLTexture2D>(
                width,
                height,
                storageFormat,
                pixelFormat,
                dataType,
                data,
                genMipMaps,
                gl_storageFormat,
                gl_pixelFormat,
                gl_dataType);

        return RHITexture2DRef(texture, &mAllocator);
    }

    RHITexture2DRef GLDriver::createTexture(uint32 width, uint32 height, EStorageFormat storageFormat)
    {
        auto texture = mAllocator.mem_new<GLTexture2D>(
                width,
                height,
                storageFormat);

        return RHITexture2DRef(texture, &mAllocator);
    }

    RHITextureCubeRef GLDriver::createTexture(uint32 size, EStorageFormat storageFormat)
    {
        return Berserk::RHITextureCubeRef();
    }

    RHIFrameBufferRef GLDriver::createFrameBuffer(uint32 width, uint32 height, EStorageFormat format)
    {
        RHITexture2DRef color = createTexture(
                width,
                height,
                format);

        RHITexture2DRef depth = createTexture(
                width,
                height,
                SF_DEPTH24,
                PF_DEPTH,
                DT_Float,
                nullptr,
                false);

        auto buffer = mAllocator.mem_new<GLFrameBufferTarget>(
                color,
                depth,
                GL_DEPTH_ATTACHMENT);

        return RHIFrameBufferRef(buffer, &mAllocator);
    }

    RHIFrameBufferRef GLDriver::createFrameBuffer(const TArray<RHITextureRef> &colorAttachments,
            const RHITexture2DRef &depthAttachment)
    {
        return Berserk::RHIFrameBufferRef();
    }

    RHIFrameBufferRef GLDriver::createFrameBuffer_DepthStencil(const TArray<RHITextureRef> &colorAttachments,
                                                               const RHITexture2DRef &depthStencilAttachment) {
        return Berserk::RHIFrameBufferRef();
    }

    RHISamplerRef GLDriver::createSampler(ESamplerFilter filterMin, ESamplerFilter filterMag, ESamplerWrapMode wrapMode)
    {
        auto gl_filterMin = GLEnums::SamplerFilter(filterMin);
        auto gl_filterMag = GLEnums::SamplerFilter(filterMag);
        auto gl_wrapMode = GLEnums::SamplerWrapMode(wrapMode);

        auto sampler = mAllocator.mem_new<GLSampler>(
                filterMin,
                filterMag,
                wrapMode,
                Vec4f(),
                gl_filterMin,
                gl_filterMag,
                gl_wrapMode);

        return RHISamplerRef(sampler, &mAllocator);
    }

    RHIUniformBufferRef GLDriver::createUniformBuffer(uint32 bindingPoint, uint32 size, const uint8 *data, EBufferUsage bufferUsage)
    {
        auto buffer = mAllocator.mem_new<GLUniformBuffer>(
                bindingPoint,
                GLEnums::BufferUsage(bufferUsage),
                size,
                data);

        return RHIUniformBufferRef(buffer, &mAllocator);
    }

    RHIDepthTestStateRef GLDriver::createDepthState(bool writeMask, ECompareFunc compareFunc)
    {
        auto state = mAllocator.mem_new<GLDepthState>(
                writeMask,
                compareFunc);

        return RHIDepthTestStateRef(state, &mAllocator);
    }

    RHIBlendStateRef GLDriver::createBlendState(EBlendFunc source, EBlendFunc destination)
    {
        auto state = mAllocator.mem_new<GLBlendState>(
                source,
                destination);

        return RHIBlendStateRef(state, &mAllocator);
    }

    RHIStencilTestStateRef
    GLDriver::createStencilState(uint32 mask, uint32 clear, ECompareFunc compare, uint32 value, uint32 read,
                                 EStencilOperation st_fail, EStencilOperation dt_fail, EStencilOperation dt_pass)
    {
        auto state = mAllocator.mem_new<GLStencilState>(
                mask,
                clear,
                compare,
                value,
                read,
                st_fail,
                dt_fail,
                dt_pass);

        return RHIStencilTestStateRef(state, &mAllocator);
    }

    RHIFaceCullingStateRef GLDriver::createFaceCullingState(EFaceCulling face, ERasterCullMode order)
    {
        auto state = mAllocator.mem_new<GLFaceCullingState>(
                face,
                order);

        return RHIFaceCullingStateRef(state, &mAllocator);
    }

    void GLDriver::setViewport(const Rect &view)
    {
        glViewport(view.x, view.y, view.width, view.height);
    }

    void GLDriver::swapBuffers()
    {

    }

    void GLDriver::bindDefaultFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLDriver::setFillMode(ERasterFillMode fillMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GLEnums::RasterFillMode(fillMode));
    }

    void GLDriver::disableDepthTest()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void GLDriver::disableStencilTest()
    {
        glDisable(GL_STENCIL_TEST);
    }

    void GLDriver::disableBlending()
    {
        glDisable(GL_BLEND);
    }

    void GLDriver::disableFaceCulling()
    {
        glDisable(GL_CULL_FACE);
    }

    const String &GLDriver::getDriverName()
    {
        return mDriverName;
    }

    const String &GLDriver::getVendorName()
    {
        return mVendorName;
    }

    const String &GLDriver::getShadingLanguageName()
    {
        return mShadingLanguageName;
    }

    void GLDriver::getHardwareInfo()
    {
        const GLubyte * renderer = glGetString(GL_RENDERER);
        const GLubyte * vendor = glGetString(GL_VENDOR);
        const GLubyte * version = glGetString(GL_VERSION);
        const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        GLint minor, major;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        char buffer[KiB] = { '\0' };

        sprintf(buffer, "%s %s", renderer, version);
        mDriverName = buffer;

        sprintf(buffer, "%s", vendor);
        mVendorName = buffer;

        sprintf(buffer, "GLSL %s", glslVersion);
        mShadingLanguageName = buffer;
    }

    void GLDriver::setClearColor(const Vec4f &color)
    {
        mClearColor = color;
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void GLDriver::clearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLDriver::setDepthClearValue(float value)
    {
        mClearDepthValue = value;
        glClearDepth(mClearDepthValue);
    }

    void GLDriver::clearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

} // namespace Berserk
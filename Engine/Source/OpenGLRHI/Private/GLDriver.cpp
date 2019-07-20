//
// Created by Egor Orachyov on 2019-07-15.
//

#include "GLDriver.h"
#include <GLResources.h>
#include <GLEnums.h>

namespace Berserk
{

    GLDriver::GLDriver(Berserk::IAllocator &allocator)
        : mAllocator(allocator),
          mUniformMapPool(GLShaderProgramBase::getMapNodeSize(), PoolAllocator::DEFAULT_CHUNK_COUNT, mAllocator)
    {
        // Setup glew - OpenGL interface provider
        // Note: that no explicit close is needed

        if (glewInit() != GLEW_OK)
        {
            DEBUG_LOG_ERROR("GLDriver: cannot initialize glew library");
            return;
        }
    }

    GLDriver::~GLDriver()
    {
        /** close */
    }

    RHIVertexShaderRef GLDriver::createVertexShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLVertexShader>(buffer);

        if (shader->isCompiled()) return RHIVertexShaderRef(shader, &mAllocator);

        return RHIVertexShaderRef();
    }

    RHIFragmentShaderRef GLDriver::createFragmentShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLFragmentShader>(buffer);

        if (shader->isCompiled()) return RHIFragmentShaderRef(shader, &mAllocator);

        return RHIFragmentShaderRef();
    }

    RHIGeometryShaderRef GLDriver::createGeometryShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLGeometryShader>(buffer);

        if (shader->isCompiled()) return RHIGeometryShaderRef(shader, &mAllocator);

        return RHIGeometryShaderRef();
    }

    RHIComputeShaderRef GLDriver::createComputeShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLComputeShader>(buffer);

        if (shader->isCompiled()) return RHIComputeShaderRef(shader, &mAllocator);

        return RHIComputeShaderRef();
    }

    RHITessControlShaderRef GLDriver::createTessellationControlShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLTessControlShader>(buffer);

        if (shader->isCompiled()) return RHITessControlShaderRef(shader, &mAllocator);

        return RHITessControlShaderRef();
    }

    RHITessEvalShaderRef GLDriver::createTessellationEvaluationShader(const char *code)
    {
        TArray<char> buffer(mAllocator);
        generateArrayWithCode(buffer, code);
        auto shader = mAllocator.engnie_new<GLTessEvalShader>(buffer);

        if (shader->isCompiled()) return RHITessEvalShaderRef(shader, &mAllocator);

        return RHITessEvalShaderRef();
    }

    RHIShaderProgramRef GLDriver::createShaderProgram(RHIVertexShaderRef &vertexShader,
            RHIFragmentShaderRef &fragmentShader,
            const RHIShaderInitializer &initializer)
    {
        auto program = mAllocator.engnie_new<GLShaderProgramVF>(
                mAllocator,
                mUniformMapPool,
                vertexShader,
                fragmentShader);

        if (!program->isLinked()) return RHIShaderProgramRef();

        addShaderProgramInfo(program, initializer);
        return RHIShaderProgramRef(program, &mAllocator);
    }

    RHIVertexBufferRef GLDriver::createVertexBuffer(uint32 size, const uint8 *data, EBufferUsage bufferUsage)
    {
        auto buffer = mAllocator.engnie_new<GLVertexBuffer>(
                GLEnums::BufferUsage(bufferUsage),
                size,
                data);

        return RHIVertexBufferRef(buffer, &mAllocator);
    }

    RHIIndexBufferRef
    GLDriver::createIndexBuffer(uint32 size, const uint8 *data, EBufferUsage bufferUsage, EIndexType indexType,
                                uint32 indexCount)
    {
        auto buffer = mAllocator.engnie_new<GLIndexBuffer>(
                GLEnums::BufferUsage(bufferUsage),
                size, data,
                GLEnums::IndexType(indexType),
                indexCount);

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
        GLuint indicesCount = gl_indexBuffer->getIndicesCount();
        GLenum gl_primitiveType = GLEnums::PrimitiveType(primitiveType);

        auto buffer = mAllocator.engnie_new<GLIndexedGeometryBuffer>(
                vertexBuffer, indexBuffer, layout, primitiveType,
                indicesCount, gl_primitiveType, indicesType);

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

        auto texture = mAllocator.engnie_new<GLTexture2D>(
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
        auto texture = mAllocator.engnie_new<GLTexture2D>(
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
        return Berserk::RHIFrameBufferRef();
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

        auto sampler = mAllocator.engnie_new<GLSampler>(
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
        auto buffer = mAllocator.engnie_new<GLUniformBuffer>(
                bindingPoint,
                GLEnums::BufferUsage(bufferUsage),
                size,
                data);

        return RHIUniformBufferRef(buffer, &mAllocator);
    }

    RHIDepthTestStateRef GLDriver::createDepthState(bool writeMask, ECompareFunc compareFunc)
    {
        return Berserk::RHIDepthTestStateRef();
    }

    RHIBlendStateRef GLDriver::createBlendState(EBlendFunc source, EBlendFunc destination)
    {
        return Berserk::RHIBlendStateRef();
    }

    RHIStencilTestStateRef
    GLDriver::createStencilState(uint32 mask, uint32 clear, ECompareFunc compare, uint32 value, uint32 read,
                                 EStencilOperation st_fail, EStencilOperation dt_fail, EStencilOperation dt_pass)
    {
        return Berserk::RHIStencilTestStateRef();
    }

    RHIFaceCullingStateRef GLDriver::createFaceCullingState(EFaceCulling face, ERasterCullMode order)
    {
        return Berserk::RHIFaceCullingStateRef();
    }

    void GLDriver::setViewport(const ViewPort &view)
    {
        glViewport(view.x, view.y, view.width, view.height);
    }

    void GLDriver::swapBuffers()
    {

    }

    void GLDriver::bindDefaultFrameBuffer()
    {
        glBindVertexArray(0);
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

    void GLDriver::generateArrayWithCode(TArray<char> &code, const char *source)
    {
        uint32 length = StringUtility<char, '\0'>::length(source);
        code.append(source, length + 1);
    }

    void GLDriver::addShaderProgramInfo(void* programPtr, const RHIShaderInitializer& initializer)
    {
        auto program = (GLShaderProgramBase*) programPtr;

        const TArray<String>& uniformNames = initializer.uniformVarNames;

        for (auto name = uniformNames.begin(); name != nullptr; name = uniformNames.next())
        {
            program->addUniformVariable(name->get());
        }

        const TArray<UniformBlockInfo>& uniformBlocks = initializer.uniformBlocksInfo;

        for (auto block = uniformBlocks.begin(); block != nullptr; block = uniformBlocks.next())
        {
            program->addUniformBlockBinding(block->name.get(), block->bindingPoint);
        }

        const TArray<SubroutineInfo>& subroutines = initializer.subroutinesInfo;

        for (auto function = subroutines.begin(); function != nullptr; function = subroutines.next())
        {
            program->addSubroutine(function->name.get(), GLEnums::ShaderType(function->shaderType));
        }
    }

} // namespace Berserk
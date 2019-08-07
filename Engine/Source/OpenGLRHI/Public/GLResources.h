//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_GLRESOURCES_H
#define BERSERK_GLRESOURCES_H

#include <RHI/RHIResources.h>
#include <Rendering/Definitions.h>
#include <GLCommon.h>
#include <GLEnums.h>
#include <Math/MathInclude.h>
#include <Logging/DebugLogMacros.h>
#include <Containers/THashMap.h>

namespace Berserk
{

    class GLUniformBuffer final : public RHIUniformBuffer
    {
    public:

        GLUniformBuffer(uint32 bindingPoint, GLenum bufferUsage,
                        uint32 size, const uint8* data);

        ~GLUniformBuffer() override;

        void update(uint32 size, const uint8 *data) override;

        void update(uint32 size, uint32 offset, const uint8 *data) override;

        void bind(uint32 bindingPoint) const override;

        void bind() const override;

        void setBindingPoint(uint32 bindingPoint) override;

        uint32 getBindingPoint() const override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;
        uint32 mBindingPoint = 0;

    };

    template <typename RHIBaseType, GLenum type>
    class GLBufferBase : public RHIBaseType
    {
    public:

        GLBufferBase(GLenum bufferUsage, uint32 size, const uint8* data);

        ~GLBufferBase() override;

        void update(uint32 size, const uint8 *data) override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        friend class GLIndexedGeometryBuffer;

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;

    };

    template<typename RHIBaseType, GLenum type>
    GLBufferBase<RHIBaseType, type>::GLBufferBase(GLenum bufferUsage, Berserk::uint32 size,
                                                  const Berserk::uint8 *data) : mBufferSize(size)
    {
        glGenBuffers(1, &mResourceID);
        glBindBuffer(type, mResourceID);
        glBufferData(type, size, data, bufferUsage);
    }

    template<typename RHIBaseType, GLenum type>
    GLBufferBase<RHIBaseType, type>::~GLBufferBase()
    {
        if (mResourceID)
        {
            glDeleteBuffers(1, &mResourceID);
            mResourceID = 0;
        }
    }

    template<typename RHIBaseType, GLenum type>
    void GLBufferBase<RHIBaseType, type>::update(uint32 size, const uint8 *data)
    {
        glBindBuffer(type, mResourceID);
        glBufferSubData(type, 0 , size, data);
        glBindBuffer(type, 0);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getSize() const
    {
        return mBufferSize;
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage() const
    {
        return sizeof(GLBufferBase<RHIBaseType, type>);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage_GPU() const
    {
        return mBufferSize;
    }

    typedef GLBufferBase<RHIVertexBuffer, GL_ARRAY_BUFFER> GLVertexBuffer;


    class GLIndexBuffer : public GLBufferBase<RHIIndexBuffer, GL_ELEMENT_ARRAY_BUFFER>
    {
    public:

        GLIndexBuffer(GLenum bufferUsage, uint32 size, const uint8* data, GLenum indexType, GLuint indexCount)
            : GLBufferBase(bufferUsage, size, data), mIndicesType(indexType), mIndicesCount(indexCount)
        {

        }

        GLenum getIndicesType() const { return mIndicesType; }

        GLuint getIndicesCount() const { return mIndicesCount; }

        uint32 getMemoryUsage() const override { return sizeof(GLIndexBuffer); }

    private:

        GLenum mIndicesType;
        GLuint mIndicesCount;

    };

    class GLSampler final : public RHISampler
    {
    public:

        GLSampler(ESamplerFilter min, ESamplerFilter mag, ESamplerWrapMode wrapMode,
                  const Vec4f &borderColor, GLint min_gl, GLint mag_gl, GLint wrapMode_gl);

        ~GLSampler() override;

        void bind(uint32 textureSlot) const override;

        ESamplerFilter getMinFilter() const override;

        ESamplerFilter getMagFilter() const override;

        ESamplerWrapMode getWrapMode() const override;

        const Vec4f &getBorderColor() const override;

    private:

        GLuint mResourceID = 0;
        ESamplerFilter mMinFilter;
        ESamplerFilter mMagFilter;
        ESamplerWrapMode mWrapMode;
        Vec4f mBorderColor;

    };

    class GLIndexedGeometryBuffer final : public RHIGeometryBuffer
    {
    public:

        GLIndexedGeometryBuffer(const RHIVertexBufferRef &vertexBuffer, const RHIIndexBufferRef &indexBuffer,
                                EDataLayout dataLayout, EPrimitiveType primitiveType, uint32 indicesCount,
                                GLenum primitiveType_gl, GLenum indexType_gl);

        ~GLIndexedGeometryBuffer() override;

        void draw() override;

        void draw(uint32 numOfElements) override;

        void draw(uint32 numOfIndices, uint32 offsetBuffer) override;

        EPrimitiveType getPrimitiveType() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        EPrimitiveType mPrimitiveType;

        GLenum mGL_primitiveType;
        GLenum mGL_indicesType;
        GLuint mResourceID = 0;
        uint32 mIndicesCount = 0;

        RHIVertexBufferRef mVertexBuffer;
        RHIIndexBufferRef mIndexBuffer;

    };

    template <typename RHIBaseClass, GLenum shaderType>
    class GLShaderBase final : public RHIBaseClass
    {
    public:

        explicit GLShaderBase(TArray<char> &code, const char* debugName = "");

        ~GLShaderBase() override;

        const char *getSourceCode() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

        bool isCompiled() const { return mResourceID != 0; }

    private:

        friend class GLShaderProgramBase;
        GLuint mResourceID;
        TArray<char> mSourceCode;

    };

    template<typename RHIBaseClass, GLenum shaderType>
    GLShaderBase<RHIBaseClass, shaderType>::GLShaderBase(Berserk::TArray<char> &code, const char* debugName)
        : mSourceCode(std::move(code))
    {
        mResourceID = glCreateShader(shaderType);

        if (mResourceID == 0)
        {
            DEBUG_LOG_ERROR("GLShaderBase: cannot create shader [name: %u]", debugName);
            return;
        }

        const char* source = getSourceCode();
        const char* sources[] = {source};

        glShaderSource(mResourceID, 1, sources, nullptr);
        glCompileShader(mResourceID);

        int32 result = 0;
        glGetShaderiv(mResourceID, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            DEBUG_LOG_ERROR("GLShaderBase: cannot compile shader [name: '%s']", debugName);

            int32 logLen;
            glGetShaderiv(mResourceID, GL_INFO_LOG_LENGTH, &logLen);

            if (logLen > 0)
            {
                char log[Buffers::SIZE_1024];
                logLen = Math::min(logLen, Buffers::SIZE_1024);

                int32 written;
                glGetShaderInfoLog(mResourceID, logLen, &written, log);

                DEBUG_LOG_ERROR("GLShaderBase: log: %s", log);
            }

            glDeleteShader(mResourceID);
            mResourceID = 0;

            return;
        }
    }

    template<typename RHIBaseClass, GLenum shaderType>
    GLShaderBase<RHIBaseClass, shaderType>::~GLShaderBase()
    {
        if (mResourceID)
        {
            glDeleteShader(mResourceID);
            mResourceID = 0;
        }
    }

    template<typename RHIBaseClass, GLenum shaderType>
    const char *GLShaderBase<RHIBaseClass, shaderType>::getSourceCode() const
    {
        return mSourceCode.getRawBuffer();
    }

    template<typename RHIBaseClass, GLenum shaderType>
    uint32 GLShaderBase<RHIBaseClass, shaderType>::getMemoryUsage() const
    {
        return sizeof(GLShaderBase<RHIBaseClass, shaderType>);
    }

    template<typename RHIBaseClass, GLenum shaderType>
    uint32 GLShaderBase<RHIBaseClass, shaderType>::getMemoryUsage_GPU() const
    {
        return sizeof(GLShaderBase<RHIBaseClass, shaderType>);
    }

    typedef GLShaderBase<RHIVertexShader, GL_VERTEX_SHADER> GLVertexShader;
    typedef GLShaderBase<RHIFragmentShader, GL_FRAGMENT_SHADER> GLFragmentShader;
    typedef GLShaderBase<RHIGeometryShader, GL_GEOMETRY_SHADER> GLGeometryShader;
    typedef GLShaderBase<RHIComputeShader, GL_COMPUTE_SHADER> GLComputeShader;
    typedef GLShaderBase<RHITessControlShader, GL_TESS_CONTROL_SHADER> GLTessControlShader;
    typedef GLShaderBase<RHITessEvalShader, GL_TESS_EVALUATION_SHADER> GLTessEvalShader;

    class GLShaderProgramBase : public RHIShaderProgram
    {
    public:

        GLShaderProgramBase(IAllocator& mapAllocator, IAllocator& mapPool);

        virtual ~GLShaderProgramBase() = default;

        void use() const override;

        void setUniform(const char *name, int32 i) const override;

        void setUniform(const char *name, uint32 i) const override;

        void setUniform(const char *name, float32 f) const override;

        void setUniform(const char *name, const Vec2f &v) const override;

        void setUniform(const char *name, const Vec3f &v) const override;

        void setUniform(const char *name, const Vec4f &v) const override;

        void setUniform(const char *name, const Mat2x2f &m) const override;

        void setUniform(const char *name, const Mat3x3f &m) const override;

        void setUniform(const char *name, const Mat4x4f &m) const override;

        void setSubroutines(EShaderType type, uint32 count, const char **functionNames) override;

        bool isLinked() const { return mResourceID != 0; }

        static uint32 getMapNodeSize();

    protected:

        template <typename RHITypeRef, typename GLShaderType>
        GLuint getShaderResourceID(const RHITypeRef& ref)
        {
            GLuint id = ((GLShaderType*)ref.pointer())->mResourceID;
            return id;
        }

        GLuint getUniformLocation(const char* name) const;

        void addUniformVariable(const char* varName);

        void addSubroutine(const char* funName, GLenum shaderType);

        void addUniformBlockBinding(const char *name, uint32 bindingPoint);

    protected:

        friend class GLDriver;
        static const int32 VALUE_NOT_FOUND = -1;
        static const uint32 SUBROUTINES_BUFFER_SIZE = 32;

        GLuint mResourceID = 0;

        typedef THashMap<String,GLuint> UniformInfoMap;
        UniformInfoMap mUniformsInfo;
        UniformInfoMap mSubroutinesInfo;

    };

    class GLShaderProgramVF final : public GLShaderProgramBase
    {
    public:

        GLShaderProgramVF(
                IAllocator& mapAllocator,
                IAllocator& mapPool,
                RHIVertexShaderRef& vertexShader,
                RHIFragmentShaderRef& fragmentShader,
                const char* debugName = "")

            : GLShaderProgramBase(mapAllocator, mapPool),
              mVertexShader(std::move(vertexShader)),
              mFragmentShader(std::move(fragmentShader))
        {
            mResourceID = glCreateProgram();
            GLuint vs_id = getShaderResourceID<RHIVertexShaderRef,GLVertexShader>(mVertexShader);
            GLuint fs_id = getShaderResourceID<RHIFragmentShaderRef,GLFragmentShader>(mFragmentShader);

            glAttachShader(mResourceID, vs_id);
            glAttachShader(mResourceID, fs_id);

            glLinkProgram(mResourceID);

            int32 status;
            glGetProgramiv(mResourceID, GL_LINK_STATUS, &status);
            if (!status)
            {
                DEBUG_LOG_ERROR("GLShaderProgramVF: cannot link shader program [name: '%s']", debugName);

                int32 logLen;
                glGetProgramiv(mResourceID, GL_INFO_LOG_LENGTH, &logLen);

                if (logLen > 0)
                {
                    char log[Buffers::SIZE_1024];
                    logLen = Math::min(logLen, Buffers::SIZE_1024);

                    GLsizei written;
                    glGetProgramInfoLog(mResourceID, logLen, &written, log);

                    DEBUG_LOG_ERROR("GLShaderProgramVF: log: %s", log);
                }

                glDeleteProgram(mResourceID);
                mResourceID = 0;

                return;
            }
        }

        ~GLShaderProgramVF() override
        {
            if (mResourceID)
            {
                GLuint vs_id = getShaderResourceID<RHIVertexShaderRef,GLVertexShader>(mVertexShader);
                GLuint fs_id = getShaderResourceID<RHIFragmentShaderRef,GLFragmentShader>(mFragmentShader);

                glDetachShader(mResourceID, vs_id);
                glDetachShader(mResourceID, fs_id);

                glDeleteProgram(mResourceID);
                mResourceID = 0;
            }
        }

    private:

        RHIVertexShaderRef mVertexShader;
        RHIFragmentShaderRef mFragmentShader;

    };

    template <typename RHIBaseType, GLenum textureType>
    class GLTexture : public RHIBaseType
    {
    public:

        void bind(uint32 textureSlot) const override
        {
            // !!! WARNING !!!
            //
            // glBindSampler(textureSlot, 0)
            //
            // Unbind any attached sampler object to this
            // texture slot. If there was any sampler binded,
            // our color layer would get wrong filtration properties,
            // therefore sampler2D would not work properly in shader.

            glBindSampler(textureSlot, 0);
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(textureType, mResourceID);
        }

        void bind(uint32 textureSlot, const RHISamplerRef &sampler) override
        {
            sampler->bind(textureSlot);
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(textureType, mResourceID);
        }

        bool isMipmapsUsed() const override
        {
            return mIsMipmapsUsed;
        }

        EStorageFormat getStorageFormat() const override
        {
            return mStorageFormat;
        }

        template <typename RHIBaseTypeRef, typename GLType>
        static GLuint getResourceID(const RHIBaseTypeRef& ref)
        {
            GLuint id = ((GLType*)ref.pointer())->mResourceID;
            return id;
        }

    protected:

        static const GLenum TEXTURE_TYPE = textureType;

        GLuint mResourceID = 0;
        bool mIsMipmapsUsed;
        EStorageFormat mStorageFormat;

    };

    class GLTexture2D : public GLTexture<RHITexture2D, GL_TEXTURE_2D>
    {
    public:

        GLTexture2D(uint32 width,
                  uint32 height,
                  EStorageFormat storageFormat);

        GLTexture2D(uint32 width,
                  uint32 height,
                  EStorageFormat storageFormat,
                  EPixelFormat pixelFormat,
                  EDataType dataType,
                  uint8* data,
                  bool genMipMaps,
                  GLenum gl_storageFormat,
                  GLenum gl_pixelFormat,
                  GLenum gl_dataType);

        ~GLTexture2D() override;

        void readData(EPixelFormat format, EDataType type, uint8 *destBuffer) const override;

        void readData(EPixelFormat format, EDataType type, uint32 lod, uint8 *destBuffer) const override;

        uint32 getWidth() const override;

        uint32 getHeight() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        uint32 mWidth;
        uint32 mHeight;

    };

    class GLFrameBufferTarget final : public RHIFrameBuffer
    {
    public:

        GLFrameBufferTarget(const RHITexture2DRef &color, const RHITexture2DRef &depth, GLenum gl_depthAttachment);

        ~GLFrameBufferTarget() override;

        void bind() override;

        uint32 getColorAttachmentsCount() const override;

        RHITextureRef getDepthAttachment() const override;

        RHITextureRef getColorAttachment(uint32 layer) const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        GLuint mResourceID = 0;
        RHITexture2DRef mColorBuffer;
        RHITexture2DRef mDepthStencil;

    };

    class GLDepthState final : public RHIDepthTestState
    {
    public:

        GLDepthState(bool writeMask, ECompareFunc compareFunc)
        {
            mWriteMask = writeMask;
            mCompareFunc = GLEnums::CompareFunc(compareFunc);
        }

        ~GLDepthState() override
        {

        }

        void enable() override
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(mCompareFunc);

            if (mWriteMask)
            {
                glDepthMask(GL_TRUE);
            }
            else
            {
                glDepthMask(GL_FALSE);
            }
        }

    private:

        GLboolean mWriteMask;
        GLenum mCompareFunc;

    };

    class GLBlendState final : public RHIBlendState
    {
    public:

        GLBlendState(EBlendFunc source,
                     EBlendFunc destination)
        {
            mBlendFuncSource = GLEnums::BlendFunc(source);
            mBlendFuncDestination = GLEnums::BlendFunc(destination);
        }

        ~GLBlendState() override
        {

        }

        void enable() override
        {
            glEnable(GL_BLEND);
            glBlendFunc(mBlendFuncSource, mBlendFuncDestination);
        }

    private:

        GLenum mBlendFuncSource;
        GLenum mBlendFuncDestination;

    };

    class GLFaceCullingState final : public RHIFaceCullingState
    {
    public:

        GLFaceCullingState(EFaceCulling face,
                           ERasterCullMode order)
        {
            mFace = GLEnums::FaceCulling(face);
            mOrder = GLEnums::RasterCullMode(order);
        }

        ~GLFaceCullingState() override
        {

        }

        void enable() override
        {
            glEnable(GL_CULL_FACE);
            glCullFace(mFace);
            glFrontFace(mOrder);
        }

    private:

        GLenum mFace;
        GLenum mOrder;

    };

    class GLStencilState final : public RHIStencilTestState
    {
    public:

        GLStencilState(uint32 mask,
                       uint32 clear,
                       ECompareFunc compare,
                       uint32 value,
                       uint32 read,
                       EStencilOperation st_fail,
                       EStencilOperation dt_fail,
                       EStencilOperation dt_pass)
        {
            mClear = clear;
            mMask = mask;
            mCompareFunc = GLEnums::CompareFunc(compare);
            mValueRef = value;
            mReadMask = read;
            mSfail = GLEnums::StencilOperation(st_fail);
            mZfail = GLEnums::StencilOperation(dt_fail);
            mZpass = GLEnums::StencilOperation(dt_pass);
        }

        ~GLStencilState() override
        {

        }

        void enable() override
        {
            glEnable(GL_STENCIL_TEST);
            glClearStencil(mClear);
            glStencilMask(mMask);
            glStencilFunc(mCompareFunc, mValueRef, mReadMask);
            glStencilOp(mSfail, mZfail, mZpass);
        }

    private:

        GLint mClear;
        GLuint mMask;
        GLenum mCompareFunc;
        GLint mValueRef;
        GLuint mReadMask;
        GLenum mSfail;
        GLenum mZfail;
        GLenum mZpass;

    };

} // namespace Berserk

#endif //BERSERK_GLRESOURCES_H
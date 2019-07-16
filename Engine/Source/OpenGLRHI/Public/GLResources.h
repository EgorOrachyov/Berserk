//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_GLRESOURCES_H
#define BERSERK_GLRESOURCES_H

#include <RHIResources.h>
#include <RHIDefinitions.h>
#include <GLCommon.h>
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

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

    private:

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;
        uint32 mBindingPoint = 0;

    };

    template <typename RHIBaseType, GLenum type>
    class GLBufferBase final : public RHIBaseType
    {
    public:

        GLBufferBase(GLenum bufferUsage, uint32 size, const uint8* data);

        ~GLBufferBase() override;

        void update(uint32 size, const uint8 *data) override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

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
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage()
    {
        return sizeof(GLBufferBase<RHIBaseType, type>);
    }

    template<typename RHIBaseType, GLenum type>
    uint32 GLBufferBase<RHIBaseType, type>::getMemoryUsage_GPU()
    {
        return mBufferSize;
    }

    typedef GLBufferBase<RHIVertexBuffer, GL_ARRAY_BUFFER> GLVertexBuffer;
    typedef GLBufferBase<RHIIndexBuffer, GL_ELEMENT_ARRAY_BUFFER> GLIndexBuffer;

    class GLSampler final : public RHISampler
    {
    public:

        GLSampler(ESamplerFilter min, ESamplerFilter mag, ESamplerWrapMode wrapMode,
                  const Vec4f &borderColor, GLint min_gl, GLint mag_gl, GLint wrapMode_gl);

        ~GLSampler() override;

        void bind(uint32 textureSlot) override;

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

        EPrimitiveType getPrimitiveType() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

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

        explicit GLShaderBase(TArray<uint8> &code, const char* debugName = "");

        ~GLShaderBase() override;

        const char *getSourceCode() const override;

        uint32 getMemoryUsage() override;

        uint32 getMemoryUsage_GPU() override;

        bool isCompiled() const { return mResourceID != 0; }

    private:

        friend class GLShaderProgramBase;
        GLuint mResourceID;
        TArray<uint8> mSourceCode;

    };

    template<typename RHIBaseClass, GLenum shaderType>
    GLShaderBase<RHIBaseClass, shaderType>::GLShaderBase(Berserk::TArray<Berserk::uint8> &code, const char* debugName)
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
        return &mSourceCode.get(0);
    }

    template<typename RHIBaseClass, GLenum shaderType>
    uint32 GLShaderBase<RHIBaseClass, shaderType>::getMemoryUsage()
    {
        return sizeof(GLShaderBase<RHIBaseClass, shaderType>);
    }

    template<typename RHIBaseClass, GLenum shaderType>
    uint32 GLShaderBase<RHIBaseClass, shaderType>::getMemoryUsage_GPU()
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

        GLShaderProgramBase(IAllocator &mapAllocator, IAllocator& mapPool);

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

    protected:

        template <typename RHITypeRef, typename GLShaderType>
        GLuint getShaderResourceID(const RHITypeRef& ref)
        {
            GLuint id = ((GLShaderType*)ref.operator->())->mResourceID;
            return id;
        }

        GLuint getUniformLocation(const char* name) const;

        void addUniformVariable(const char* varName);

        void addSubroutine(const char* funName, GLenum shaderType);

        void addUniformBlockBinding(const char *name, uint32 bindingPoint);

    protected:

        friend class GLDriver;
        static const uint32 VALUE_NOT_FOUND = 0;
        static const uint32 SUBROUTINES_BUFFER_SIZE = 32;

        GLuint mResourceID = 0;

        THashMap<String,GLuint> mUniformsInfo;
        THashMap<String,GLuint> mSubroutinesInfo;

    };

    class GLShaderProgramVF final : public GLShaderProgramBase
    {
    public:

        GLShaderProgramVF(
                IAllocator &mapAllocator,
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


} // namespace Berserk

#endif //BERSERK_GLRESOURCES_H
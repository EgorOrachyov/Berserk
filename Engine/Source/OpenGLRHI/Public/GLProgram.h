//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLPROGRAM_H
#define BERSERK_GLPROGRAM_H

#include <RHI/RHIShaderProgram.h>
#include <GLCommon.h>
#include <GLShader.h>
#include <Containers/THashMap.h>

namespace Berserk
{

    class GLShaderProgramBase : public RHIShaderProgram
    {
    public:

        GLShaderProgramBase(IAllocator& mapAllocator, IAllocator& mapPool);

        ~GLShaderProgramBase() override = default;

        void setProgramData(const GpuProgramData& data) override;

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

        GpuProgramData* programData;

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

} // namespace Berserk

#endif //BERSERK_GLPROGRAM_H

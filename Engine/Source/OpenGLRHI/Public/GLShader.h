//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLSHADER_H
#define BERSERK_GLSHADER_H

namespace Berserk
{

    template <typename RHIBaseClass, GLenum shaderType>
    class GLShaderBase final : public RHIBaseClass
    {
    public:

        explicit GLShaderBase(const char* code, const char* debugName = "");

        ~GLShaderBase() override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

        bool isCompiled() const { return mResourceID != 0; }

    private:

        friend class GLShaderProgramBase;
        GLuint mResourceID;

    };

    template<typename RHIBaseClass, GLenum shaderType>
    GLShaderBase<RHIBaseClass, shaderType>::GLShaderBase(const char* code, const char* debugName)
    {
        mResourceID = glCreateShader(shaderType);

        if (mResourceID == 0)
        {
            DEBUG_LOG_ERROR("GLShaderBase: cannot create shader [name: %u]", debugName);
            return;
        }

        const char* source = code;
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

} // namespace Berserk

#endif //BERSERK_GLSHADER_H

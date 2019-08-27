//
// Created by Egor Orachyov on 2019-08-27.
//

#include <GLProgram.h>
#include <GLEnums.h>
#include <Misc/Assert.h>
#include <Math/MathInclude.h>
#include <Misc/ExceptionMacros.h>

namespace Berserk
{

    GLShaderProgramVF::GLShaderProgramVF(RHIVertexShaderRef vertexShader,
                                         RHIFragmentShaderRef fragmentShader,
                                         const char* debugName)
            : GLShaderProgramBase(),
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

    GLShaderProgramVF::~GLShaderProgramVF()
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

} // namespace Berserk
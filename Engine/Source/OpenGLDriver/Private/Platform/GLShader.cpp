//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Platform/GLShader.h"
#include "Platform/GLInclude.h"

namespace Berserk
{

    void GLShader::initialize(const char *name)
    {
        mGPUProgramSize = 0;
        mProgram = 0;

        for (uint32 i = 0; i < GLRenderDriver::MAX_SHADER_COUNT; i++)
        {
            mShaders[i] = 0;
        }

        mReferenceCount = 0;
        mResourceName = name;

        new(&mUniformMap) HashMap<CName,uint32>(CName::Hashing);
    }

    void GLShader::addReference()
    {
        mReferenceCount += 1;
    }

    void GLShader::release()
    {
        if (mReferenceCount > 0)
        {
            mReferenceCount -= 1;
        }

        if (mReferenceCount == 0 && mProgram)
        {
            PUSH("GLShader: delete [name: '%s']", mResourceName.get());

            for (uint32 i = 0; i < GLRenderDriver::MAX_SHADER_COUNT; i++)
            {
                if (mShaders[i]) { glDeleteShader(mShaders[i]); }
                mShaders[i] = 0;
            }

            glDeleteProgram(mProgram);
            mProgram = 0;

            delete (&mUniformMap);
        }
    }

    uint32 GLShader::getReferenceCount()
    {
        return mReferenceCount;
    }

    uint32 GLShader::getMemoryUsage()
    {
        return sizeof(GLShader) + mUniformMap.getMemoryUsage();
    }

    const char* GLShader::getName()
    {
        return mResourceName.get();
    }

    void GLShader::createProgram()
    {
        mProgram = glCreateProgram();
        FAIL(mProgram, "Cannot create GL GPU program [name: '%s']", mResourceName.get());
    }

    void GLShader::attachShader(IRenderDriver::ShaderType shaderType, const char *source, const char *filename)
    {
        if (!mProgram)
        {
            ERROR("An attempt to load shader to not initialized GPU program [file: %s] [name: '%s']", filename, mResourceName.get());
            return;
        }

        {
            // Profile memory usage of one shader, attached to our
            // Shader program. It is an asymptotic approximation of
            // number of bytes, used on gpu side to represent program

            mGPUProgramSize += Strings<char,'\0'>::strlen(source);
        }

        uint32 type;
        uint32 glType = GLRenderDriver::getShaderType(shaderType);

        switch (glType)
        {
            case GLRenderDriver::VERTEX: type = 0;
                break;

            case GLRenderDriver::GEOMETRY: type = 1;
                break;

            case GLRenderDriver::TESSELLATION_CONTROL: type = 2;
                break;

            case GLRenderDriver::TESSELLATION_EVALUATION: type = 3;
                break;

            case GLRenderDriver::FRAGMENT: type = 4;
                break;

            case GLRenderDriver::COMPUTE: type = 5;
                break;

            default:
                FAIL(false, "Invalid shader type identifier");
        }

        mShaders[type] = glCreateShader(glType);
        FAIL(mShaders[type], "Cannot create GL GPU shader [file: %s] [name: '%s']", filename, mResourceName.get());

        auto shader = mShaders[type];

        const char* sources[] = {source};
        glShaderSource(shader, 1, sources, nullptr);

        glCompileShader(shader);

        int32 result = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            ERROR("Cannot compile shader [file: %s] [name: '%s']", filename, mResourceName.get());

            int32 logLen;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

            if (logLen > 0)
            {
                char buffer[Buffers::SIZE_1024];
                logLen = Math::min(logLen, Buffers::SIZE_1024);

                int32 written;
                glGetShaderInfoLog(shader, Buffers::SIZE_1024, &written, buffer);

                PUSH("Shader log [file: %s] [name: '%s']", filename, mResourceName.get());
                PUSH("%s", buffer);
            }

            glDeleteShader(shader);
            mShaders[type] = 0;

            return;
        }

        glAttachShader(mProgram, shader);
    }

    void GLShader::link()
    {
        if (!mProgram)
        {
            ERROR("An attempt to link not initialized GPU program [name: '%s']", mResourceName.get());
            return;
        }

        glLinkProgram(mProgram);

        int32 status;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
        if (!status)
        {
            ERROR("Cannot link GL GPU program");

            int32 logLen;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLen);

            if (logLen > 0)
            {
                char log[Buffers::SIZE_1024];
                logLen = Math::min(logLen, Buffers::SIZE_1024);

                GLsizei written;
                glGetProgramInfoLog(mProgram, logLen, &written, log);

                PUSH("Shader program log");
                PUSH("%s", log);
            }

            return;
        }
    }

    void GLShader::validate()
    {
        if (!mProgram)
        {
            return;
        }

        int32 status;
        glValidateProgram(mProgram);
        glGetProgramiv(mProgram, GL_VALIDATE_STATUS, &status);

        if (GL_FALSE == status)
        {

            int32 length = 0;
            char log[Buffers::SIZE_1024];

            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);

            if (length > 0)
            {
                length = Math::min(length, Buffers::SIZE_1024);
                int32 written = 0;
                glGetProgramInfoLog(mProgram, length, &written, log);

                PUSH("Shader program validation");
                PUSH("%s", log);
            }
        }
    }

    void GLShader::use()
    {
        glUseProgram(mProgram);
    }

    void GLShader::addUniformVariable(const char *name)
    {
        int32 location = glGetUniformLocation(mProgram, name);

        if (location == NOT_FOUND)
        {
            WARNING("Cannot find uniform location %s", name);
            return;
        }

        mUniformMap.add(CName(name), (uint32)location);
    }

    void GLShader::bindAttributeLocation(uint32 location, const char *name)
    {
        glBindAttribLocation(mProgram, location, name);
    }

    void GLShader::bindFragmentDataLocation(uint32 location, const char *name)
    {
        glBindFragDataLocation(mProgram, location, name);
    }
    void GLShader::setUniform(const char *name, int32 i)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform1i(*location, i);
    }

    void GLShader::setUniform(const char *name, uint32 i)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform1ui(*location, i);
    }

    void GLShader::setUniform(const char *name, float32 f)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform1f(*location, f);
    }

    void GLShader::setUniform(const char *name, const Vec2f &v)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform2f(*location, v.x, v.y);
    }

    void GLShader::setUniform(const char *name, const Vec3f &v)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform3f(*location, v.x, v.y, v.z);
    }

    void GLShader::setUniform(const char *name, const Vec4f &v)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniform4f(*location, v.x, v.y, v.z, v.w);
    }

    void GLShader::setUniform(const char *name, const Mat2x2f &m)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniformMatrix2fv(*location, 1, GL_TRUE, m.get());
    }

    void GLShader::setUniform(const char *name, const Mat3x3f &m)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniformMatrix3fv(*location, 1, GL_TRUE, m.get());
    }

    void GLShader::setUniform(const char *name, const Mat4x4f &m)
    {
        auto location = mUniformMap[CName(name)];
        FAIL(location, "Attempt to access unresolved uniform variable [name: %s]", name);
        glUniformMatrix4fv(*location, 1, GL_TRUE, m.get());
    }

    void GLShader::setSubroutines(uint32 shaderType, uint32 count, uint32 *indices)
    {
        glUniformSubroutinesuiv(shaderType, count, indices);
    }

    void GLShader::setUniformBlockBinding(const char *name, uint32 bindingPoint)
    {
        auto location = getUniformBlockIndex(name);

        if (location == NOT_FOUND)
        {
            WARNING("Cannot find uniform block location %s", name);
            return;
        }

        glUniformBlockBinding(mProgram, (uint32)location, bindingPoint);
    }

    int32 GLShader::getUniformLocation(const char *name)
    {
        int32 location = glGetUniformLocation(mProgram, name);
        if (location == NOT_FOUND) WARNING("Cannot find uniform location %s", name);
        return location;
    }

    int32 GLShader::getUniformBlockIndex(const char *name)
    {
        int32 location = glGetUniformBlockIndex(mProgram, name);
        if (location == NOT_FOUND) WARNING("Cannot find uniform block index %s", name);
        return location;
    }

    int32 GLShader::getAttributeLocation(const char *name)
    {
        int32 location = glGetAttribLocation(mProgram, name);
        if (location == NOT_FOUND) WARNING("Cannot find attribute location %s", name);
        return location;
    }

    int32 GLShader::getSubroutineLocation(uint32 shaderType, const char *name)
    {
        int32 location = glGetSubroutineUniformLocation(mProgram, shaderType, name);
        if (location == NOT_FOUND) WARNING("Cannot find subroutine location %s", name);
        return location;
    }

    uint32 GLShader::getSubroutineIndex(uint32 shaderType, const char *name)
    {
        uint32 location = glGetSubroutineIndex(mProgram, shaderType, name);
        if (location == GL_INVALID_INDEX) WARNING("Cannot find subroutine index %s", name);
        return location;
    }

    uint32 GLShader::getGPUMemoryUsage()
    {
        return mGPUProgramSize;
    }
    

} // namespace Berserk
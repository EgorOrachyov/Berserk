//
// Created by Egor Orachyov on 04.07.2018.
//

#include "GLGPUProgram.h"

#include "../../Core/Essential/Assert.h"
#include "../../Core/Logging/LogMessages.h"
#include "../../Core/Math/UtilityNumbers.h"


namespace Berserk
{

    GLGPUProgram::GLGPUProgram()
    {
        mHandle = 0;
        mIsCreated = false;
        mIsLinked = false;
        mIsCompiled = false;
    }

    GLGPUProgram::~GLGPUProgram()
    {
        destroy();
    }

    void GLGPUProgram::init()
    {
        mHandle = glCreateProgram();
        if (!mHandle)
        {
            ERROR("Cannot create GL GPU program");
            mIsCreated = false;
        }
        else
        {
            mIsCreated = true;
        }
    }

    void GLGPUProgram::destroy()
    {
        if (mIsCreated)
        {
            // Query the number of attached shaders
            GLint numOfHandles = 0;
            glGetProgramiv(mHandle, GL_ATTACHED_SHADERS, &numOfHandles);
            numOfHandles = min(numOfHandles, BUFFER_SIZE_32);

            // Get the shader handles
            GLuint shaderHandles[BUFFER_SIZE_32];
            glGetAttachedShaders(mHandle, numOfHandles, NULL, shaderHandles);

            // Delete shaders
            for (int i = 0; i < numOfHandles; i++)
            {
                glDeleteShader(shaderHandles[i]);
            }

            glDeleteProgram(mHandle);
        }
        mIsLinked = false;
        mIsCreated = false;
        mIsCompiled = false;
    }

    void GLGPUProgram::compileShader(const CHAR* filename, GLShaderType type)
    {
        if (mIsCreated)
        {
            /// Load shader text from file

            CHAR buffer[BUFFER_SIZE_2048];

            FILE* file = fopen(filename, "r");
            if (!file)
            {
                ERROR("Cannot open file with name %s", filename);
                mIsCompiled = false;
                return;
            }

            int32 read = 0;
            while(!feof(file))
            {
                buffer[read++] = fgetc(file);
            }
            buffer[--read] = '\0';
            // debug
            // fprintf(stdout, "%s\n", buffer);
            fclose(file);

            /// Create shader

            GLuint shader = glCreateShader(type);
            if (!shader)
            {
                ERROR("Cannot create GL GPU shader from file %s", filename);
                mIsCompiled = false;
                return;
            }

            const CHAR* shaderSource[] = {buffer};
            glShaderSource(shader, 1, shaderSource, NULL);

            /// Compile shader and check compilation result

            glCompileShader(shader);

            GLint result;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
            if (!result)
            {
                ERROR("Cannot compile shader from file %s", filename);

                GLint logLen;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

                if (logLen > 0)
                {
                    logLen = min(logLen, BUFFER_SIZE_2048);

                    GLsizei written;
                    glGetShaderInfoLog(shader, BUFFER_SIZE_2048, &written, buffer);

                    OPEN_BLOCK("Shader log %s", filename);
                    PUSH_BLOCK("%s", buffer);
                    CLOSE_BLOCK();
                }

                glDeleteShader(shader);
                mIsCompiled = false;
                return;
            }

            /// Attach shader to our program

            glAttachShader(mHandle, shader);
            mIsCompiled = true;
        }
        else
        {
            WARNING("An attempt to load shader to not initialized GPU program");
        }
    }

    void GLGPUProgram::link()
    {
        if (mIsLinked)
        {
            return;
        }
        if (mIsCreated)
        {
            /// Link all the attached shaders to our program

            glLinkProgram(mHandle);

            GLint status;
            glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
            if (!status)
            {
                ERROR("Cannot link GL GPU program");

                GLint logLen;
                glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &logLen);

                if (logLen > 0)
                {
                    char log[BUFFER_SIZE_512];
                    logLen = min(logLen, BUFFER_SIZE_512);

                    GLsizei written;
                    glGetProgramInfoLog(mHandle, logLen, &written, log);

                    OPEN_BLOCK("Shader program log");
                    PUSH_BLOCK("%s", log);
                    CLOSE_BLOCK();
                }

                mIsLinked = false;
                return;
            }

            mIsLinked = true;
        }
        else
        {
            WARNING("An attempt to link not initialized GPU program");
        }
    }

    void GLGPUProgram::use()
    {
        glUseProgram(mHandle);
    }

    void GLGPUProgram::validate()
    {
        if (!mIsLinked)
        {
            return;
        }

        GLint status;
        glValidateProgram(mHandle);
        glGetProgramiv(mHandle, GL_VALIDATE_STATUS, &status);

        if (GL_FALSE == status)
        {

            int length = 0;
            CHAR log[BUFFER_SIZE_512];

            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            if (length > 0)
            {
                length = min(length, BUFFER_SIZE_512);
                int written = 0;
                glGetProgramInfoLog(mHandle, length, &written, log);

                PUSH("GL GPU Program log: %s", log);
            }
        }
    }

    void GLGPUProgram::bindAttributeLocation(GLuint location, const CHAR* name)
    {
        if (mIsCreated && !mIsLinked)
        {
            glBindAttribLocation(mHandle, location, name);
        }
    }

    void GLGPUProgram::bindFragmentDataLocation(GLuint location, const CHAR* name)
    {
        if (mIsCreated && !mIsLinked)
        {
            glBindFragDataLocation(mHandle, location, name);
        }
    }

    void GLGPUProgram::setUniform(const CHAR* name, const int32 i) const
    {
        glUniform1i(getUniformLocation(name), i);
    }

    void GLGPUProgram::setUniform(const CHAR *name, uint32 i) const
    {
        glUniform1ui(getUniformLocation(name), i);
    }

    void GLGPUProgram::setUniform(const CHAR* name, const float32 f) const
    {
        glUniform1f(getUniformLocation(name), f);
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Vector2f& v) const
    {
        glUniform2f(getUniformLocation(name), v.x, v.y);
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Vector3f& v) const
    {
        glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Vector4f& v) const
    {
        glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Matrix2x2f& m) const
    {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_TRUE, m.getArray());
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Matrix3x3f& m) const
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_TRUE, m.getArray());
    }

    void GLGPUProgram::setUniform(const CHAR* name, const Matrix4x4f& m) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, m.getArray());
    }

    GLint GLGPUProgram::getUniformLocation(const CHAR* name) const
    {
        return glGetUniformLocation(mHandle, name);
    }

    GLint GLGPUProgram::getAttributeLocation(const CHAR* name) const
    {
        return glGetAttribLocation(mHandle, name);
    }

    bool GLGPUProgram::isLinked() const
    {
        return mIsLinked;
    }

    bool GLGPUProgram::isCreated() const
    {
        return mIsCreated;
    }

    bool GLGPUProgram::isCompiled() const
    {
        return mIsCompiled;
    }

    void GLGPUProgram::printActiveAttributes() const
    {
        if (mIsLinked)
        {
        #ifdef TARGET_PLATFORM_MACOS

            GLint written, size, location, maxLength, numAttribs;
            GLenum type;

            glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
            glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &numAttribs);

            OPEN_BLOCK("Active attributes");
            PUSH_BLOCK("Location Name Type");
            for(int i = 0; i < numAttribs; i++)
            {
                char name[BUFFER_SIZE_256];
                glGetActiveAttrib(mHandle, i, maxLength, &written, &size, &type, name);
                location = glGetAttribLocation(mHandle, name);

                PUSH_BLOCK("%3d %s (%s)", location, name, getStringType(type));
            }
            CLOSE_BLOCK();

        #else

            GLint numAttributes;
            glGetProgramInterfaceiv(mHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttributes);

            GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};

            OPEN_BLOCK("Active attributes");
            PUSH_BLOCK("Location Name Type");
            for(int i = 0; i < numAttributes; i++)
            {
                GLint results[3];
                glGetProgramResourceiv(mHandle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

                GLint nameBufferSize = results[0] + 1;
                char name[STRING_SIZE_256];
                glGetProgramResourceName(mHandle, GL_PROGRAM_INPUT, i, nameBufferSize, NULL, name);

                PUSH_BLOCK("%3d %s (%s)", results[2], name, getStringType(results[1]));
            }
            CLOSE_BLOCK();

        #endif
        }
    }

    void GLGPUProgram::printActiveUniforms() const
    {
        if (mIsLinked)
        {
        #ifdef TARGET_PLATFORM_MACOS

            GLint numUniforms, size, location, maxLen;
            GLsizei written;
            GLenum type;

            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &numUniforms);

            OPEN_BLOCK("Active uniforms");
            PUSH_BLOCK("Location Name Type");
            for(int i = 0; i < numUniforms; i++)
            {
                char name[BUFFER_SIZE_256];
                glGetActiveUniform(mHandle, i, maxLen, &written, &size, &type, name);
                location = glGetUniformLocation(mHandle, name);

                if (location == -1)
                    continue;  // Skip uniforms in blocks

                PUSH_BLOCK("%3d %s (%s)", location, name, getStringType(type));
            }
            CLOSE_BLOCK();

        #else

            GLint numUniforms = 0;
            glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

            GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

            OPEN_BLOCK("Active uniforms");
            PUSH_BLOCK("Location Name Type");
            for(int i = 0; i < numUniforms; i++)
            {
                GLint results[4];
                glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

                if (results[3] != -1)
                    continue;  // Skip uniforms in blocks

                GLint nameBufSize = results[0] + 1;
                char name[STRING_SIZE_256];
                glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);

                PUSH_BLOCK("%3d %s (%s)", results[2], name, getStringType(results[1]));
            }
            CLOSE_BLOCK();

        #endif
        }
    }

    void GLGPUProgram::printActiveUniformsInBlocks() const
    {
        if (mIsLinked)
        {
        #ifdef TARGET_PLATFORM_MACOS

            GLint written, maxLength, maxUniLen, nBlocks, binding;

            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxLength);
            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_BLOCKS, &nBlocks);
            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniLen);

            maxLength = min(maxLength, BUFFER_SIZE_256);
            maxUniLen = min(maxUniLen, BUFFER_SIZE_256);

            GLchar uniName[BUFFER_SIZE_256];
            GLchar name[BUFFER_SIZE_256];

            OPEN_BLOCK("Active uniform blocks");
            for (GLuint i = 0; i < nBlocks; i++)
            {
                glGetActiveUniformBlockName(mHandle, i, maxLength, &written, name);
                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_BINDING, &binding);

                PUSH_BLOCK("Uniform block \"%s\" (%d):", name, binding);

                GLint nUnis;
                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nUnis);
                GLint unifIndexes[BUFFER_SIZE_64];
                glGetActiveUniformBlockiv(mHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, unifIndexes);

                nUnis = min(nUnis, BUFFER_SIZE_64);
                for (int unif = 0; unif < nUnis; unif++)
                {
                    GLuint uniIndex = (GLuint)unifIndexes[unif];
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(mHandle, uniIndex, maxUniLen, &written, &size, &type, uniName);
                    PUSH_BLOCK("   %s (%s)", uniName, getStringType(type));
                }
            }
            CLOSE_BLOCK();

        #else
            GLint numBlocks = 0;

            glGetProgramInterfaceiv(mHandle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
            GLenum blockProps[] = {GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH};
            GLenum blockIndex[] = {GL_ACTIVE_VARIABLES};
            GLenum props[] = {GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX};

            OPEN_BLOCK("Active uniform blocks");
            for(int block = 0; block < numBlocks; ++block)
            {
                GLint blockInfo[2];
                glGetProgramResourceiv(mHandle, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
                GLint numUnis = blockInfo[0];

                char * blockName = new char[blockInfo[1]+1];
                glGetProgramResourceName(mHandle, GL_UNIFORM_BLOCK, block, blockInfo[1]+1, NULL, blockName);
                PUSH_BLOCK("Uniform block \"%s\":", blockName);
                delete [] blockName;

                GLint * unifIndexes = new GLint[numUnis];
                glGetProgramResourceiv(mHandle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

                for( int unif = 0; unif < numUnis; ++unif )
                {
                    GLint uniIndex = unifIndexes[unif];
                    GLint results[3];
                    glGetProgramResourceiv(mHandle, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);

                    GLint nameBufSize = results[0] + 1;
                    char * name = new char[nameBufSize];
                    glGetProgramResourceName(mHandle, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);
                    PUSH_BLOCK("   %s (%s)\n", name, getStringType(results[1]));
                    delete [] name;
                }

                delete [] unifIndexes;
            }
        #endif
        }
    }

    const CHAR* GLGPUProgram::getStringType(GLenum type) const
    {
        switch(type) {
            case GL_FLOAT:
                return "float";
            case GL_FLOAT_VEC2:
                return "vec2";
            case GL_FLOAT_VEC3:
                return "vec3";
            case GL_FLOAT_VEC4:
                return "vec4";
            case GL_DOUBLE:
                return "double";
            case GL_INT:
                return "int";
            case GL_UNSIGNED_INT:
                return "unsigned int";
            case GL_BOOL:
                return "bool";
            case GL_FLOAT_MAT2:
                return "mat2";
            case GL_FLOAT_MAT3:
                return "mat3";
            case GL_FLOAT_MAT4:
                return "mat4";
            default:
                return "?";
        }
    }

} // namespace Berserk
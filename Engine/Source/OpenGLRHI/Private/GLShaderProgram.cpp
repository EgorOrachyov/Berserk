//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLResources.h>
#include <Misc/Assert.h>

namespace Berserk
{

    GLShaderProgramBase::GLShaderProgramBase(Berserk::IAllocator &mapAllocator, Berserk::IAllocator &mapPool)
        : mUniformsInfo(mapAllocator, mapPool), mSubroutinesInfo(mapAllocator, mapPool)
    {
        mUniformsInfo.setHashFunction(String::hash);
        mSubroutinesInfo.setHashFunction(String::hash);
    }

    void GLShaderProgramBase::use() const
    {
        glUseProgram(mResourceID);
    }

    void GLShaderProgramBase::setUniform(const char *name, int32 i) const
    {
        GLuint location = getUniformLocation(name);
        glUniform1i(location, i);
    }

    void GLShaderProgramBase::setUniform(const char *name, uint32 i) const
    {
        GLuint location = getUniformLocation(name);
        glUniform1ui(location, i);
    }

    void GLShaderProgramBase::setUniform(const char *name, float32 f) const
    {
        GLuint location = getUniformLocation(name);
        glUniform1f(location, f);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec2f &v) const
    {
        GLuint location = getUniformLocation(name);
        glUniform2f(location, v.x, v.y);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec3f &v) const
    {
        GLuint location = getUniformLocation(name);
        glUniform3f(location, v.x, v.y, v.z);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec4f &v) const
    {
        GLuint location = getUniformLocation(name);
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat2x2f &m) const
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix2fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat3x3f &m) const
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat4x4f &m) const
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setSubroutines(EShaderType type, uint32 count, const char **functionNames)
    {
        assertion(count <= SUBROUTINES_BUFFER_SIZE);
        GLuint buffer[SUBROUTINES_BUFFER_SIZE];

        for (uint32 i = 0; i < count; i++)
        {
            GLuint* location = mSubroutinesInfo.get(Wrapper(functionNames[i]));
            assertion_msg(location != nullptr, "GLShaderProgramBase: cannot find subroutine location [name: %s]", functionNames[i]);
            buffer[i] = *location;
        }

        GLenum shaderType;

        switch (type)
        {
            case EShaderType::ST_Vertex:
                shaderType = GL_VERTEX_SHADER;
                break;
            case EShaderType::ST_Fragment:
                shaderType = GL_FRAGMENT_SHADER;
                break;
            case EShaderType::ST_Geometry:
                shaderType = GL_GEOMETRY_SHADER;
                break;
            case EShaderType::ST_Compute:
                shaderType = GL_COMPUTE_SHADER;
                break;
            case EShaderType::ST_TessellationControl:
                shaderType = GL_TESS_CONTROL_SHADER;
                break;
            case EShaderType::ST_TessellationEvaluation:
                shaderType = GL_TESS_EVALUATION_SHADER;
                break;
            default:
                DEBUG_LOG_ERROR("GLShaderProgramBase: invalid shader type");
                return;
        }

        glUniformSubroutinesuiv(shaderType, count, buffer);
    }

    GLuint GLShaderProgramBase::getUniformLocation(const char *name) const
    {
        GLuint* location = mUniformsInfo.get(Wrapper(name));
        assertion_msg(location != nullptr, "GLShaderProgramBase: cannot find uniform location [name: %s]", name);
        return *location;
    }

    void GLShaderProgramBase::addUniformVariable(const char *varName)
    {
        GLint location = glGetUniformLocation(mResourceID, varName);
        assertion_msg(location != VALUE_NOT_FOUND, "GLShaderProgramBase: cannot find uniform location [name: %s]", varName);

        mUniformsInfo.emplace(varName, (GLuint) location);
    }

    void GLShaderProgramBase::addSubroutine(const char *funName, GLenum shaderType)
    {
        GLuint location = glGetSubroutineIndex(mResourceID, shaderType, funName);
        assertion_msg(location != GL_INVALID_INDEX, "GLShaderProgramBase: cannot find subroutine index [name: %s]", funName);

        mSubroutinesInfo.emplace(funName, location);
    }

    void GLShaderProgramBase::addUniformBlockBinding(const char *name, uint32 bindingPoint)
    {
        GLint location = glGetUniformBlockIndex(mResourceID, name);
        assertion_msg(location != VALUE_NOT_FOUND, "GLShaderProgramBase: cannot find uniform block location [name: %s]", name);

        glUniformBlockBinding(mResourceID, (GLuint) location, bindingPoint);
    }

    uint32 GLShaderProgramBase::getMapNodeSize()
    {
        return UniformInfoMap::getNodeSize();
    }

} // namespace Berserk
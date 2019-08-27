//
// Created by Egor Orachyov on 2019-07-15.
//

#include <GLProgram.h>
#include <GLEnums.h>
#include <Misc/Assert.h>
#include <Math/MathInclude.h>
#include <Misc/ExceptionMacros.h>

namespace Berserk
{

    void GLShaderProgramBase::setProgramData(GpuProgramData &data)
    {
        mProgramData = &data;

        for (auto desc = mProgramData->variables.begin(); desc != nullptr; desc = mProgramData->variables.next())
        { addUniformVariable(*desc); }

        for (auto desc = mProgramData->blocks.begin(); desc != nullptr; desc = mProgramData->blocks.next())
        { addUniformBlock(*desc); }

        for (auto desc = mProgramData->textures.begin(); desc != nullptr; desc = mProgramData->textures.next())
        { addUniformTexture(*desc); }

        for (auto desc = mProgramData->subroutines.begin(); desc != nullptr; desc = mProgramData->subroutines.next())
        { addSubroutine(*desc); }
    }

    void GLShaderProgramBase::use() const
    {
        glUseProgram(mResourceID);
    }

    void GLShaderProgramBase::setUniform(const char *name, int32 i) const
    {
        GLuint location = getUniformVariable(name);
        glUniform1i(location, i);
    }

    void GLShaderProgramBase::setUniform(const char *name, uint32 i) const
    {
        GLuint location = getUniformVariable(name);
        glUniform1ui(location, i);
    }

    void GLShaderProgramBase::setUniform(const char *name, float32 f) const
    {
        GLuint location = getUniformVariable(name);
        glUniform1f(location, f);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec2f &v) const
    {
        GLuint location = getUniformVariable(name);
        glUniform2f(location, v.x, v.y);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec3f &v) const
    {
        GLuint location = getUniformVariable(name);
        glUniform3f(location, v.x, v.y, v.z);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Vec4f &v) const
    {
        GLuint location = getUniformVariable(name);
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat2x2f &m) const
    {
        GLuint location = getUniformVariable(name);
        glUniformMatrix2fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat3x3f &m) const
    {
        GLuint location = getUniformVariable(name);
        glUniformMatrix3fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setUniform(const char *name, const Mat4x4f &m) const
    {
        GLuint location = getUniformVariable(name);
        glUniformMatrix4fv(location, 1, GL_TRUE, m.get());
    }

    void GLShaderProgramBase::setTexture(const char *name, Berserk::uint32 i) const
    {
        GLuint location = getUniformTexure(name);
        glUniform1ui(location, i);
    }

    void GLShaderProgramBase::setSubroutines(EShaderType type, uint32 count, const char **functionNames)
    {
        assertion(count <= SUBROUTINES_BUFFER_SIZE);
        GLuint buffer[SUBROUTINES_BUFFER_SIZE];

        for (uint32 i = 0; i < count; i++)
        {
            buffer[i] = getSubroutine(functionNames[i]);
        }

        GLenum shaderType = GLEnums::ShaderType(type);
        glUniformSubroutinesuiv(shaderType, count, buffer);
    }

    void GLShaderProgramBase::addUniformVariable(GpuParamDesc &desc)
    {
        GLint location = glGetUniformLocation(mResourceID, desc.name.get());
        assertion_msg(location != VALUE_NOT_FOUND, "GLShaderProgramBase: cannot find uniform var location [name: %s]", desc.name.get());

        desc.index = static_cast<uint32>(location);
    }

    void GLShaderProgramBase::addUniformTexture(Berserk::GpuParamDesc &desc)
    {
        GLint location = glGetUniformLocation(mResourceID, desc.name.get());
        assertion_msg(location != VALUE_NOT_FOUND, "GLShaderProgramBase: cannot find uniform texture location [name: %s]", desc.name.get());

        desc.index = static_cast<uint32>(location);
    }

    void GLShaderProgramBase::addUniformBlock(Berserk::GpuBlockDesc &desc)
    {
        GLint location = glGetUniformBlockIndex(mResourceID, desc.name.get());
        assertion_msg(location != VALUE_NOT_FOUND, "GLShaderProgramBase: cannot find uniform block location [name: %s]", desc.name.get());

        desc.index = static_cast<uint32>(location);
        glUniformBlockBinding(mResourceID, (GLuint) location, desc.binding);
    }

    void GLShaderProgramBase::addSubroutine(Berserk::GpuSubroutineDesc &desc)
    {
        GLuint location = glGetSubroutineIndex(mResourceID, GLEnums::ShaderType(desc.type), desc.name.get());
        assertion_msg(location != GL_INVALID_INDEX, "GLShaderProgramBase: cannot find subroutine index [name: %s]", desc.name.get());

        desc.index = static_cast<uint32>(location);
    }

    GLuint GLShaderProgramBase::getUniformVariable(const char *name) const
    {
        for (auto desc = mProgramData->variables.begin(); desc != nullptr; desc = mProgramData->variables.next())
        {
            if (desc->name == name)
                return desc->index;
        }

        engine_exception("GLShaderProgramBase: no such uniform variable [name: %s]", name);
    }

    GLuint GLShaderProgramBase::getUniformBlock(const char *name) const
    {
        for (auto desc = mProgramData->blocks.begin(); desc != nullptr; desc = mProgramData->blocks.next())
        {
            if (desc->name == name)
                return desc->index;
        }

        engine_exception("GLShaderProgramBase: no such uniform block [name: %s]", name);
    }

    GLuint GLShaderProgramBase::getUniformTexure(const char *name) const
    {
        for (auto desc = mProgramData->textures.begin(); desc != nullptr; desc = mProgramData->textures.next())
        {
            if (desc->name == name)
                return desc->index;
        }

        engine_exception("GLShaderProgramBase: no such uniform texture [name: %s]", name);
    }

    GLuint GLShaderProgramBase::getSubroutine(const char *name) const
    {
        for (auto desc = mProgramData->subroutines.begin(); desc != nullptr; desc = mProgramData->subroutines.next())
        {
            if (desc->name == name)
                return desc->index;
        }

        engine_exception("GLShaderProgramBase: no such subroutine [name: %s]", name);
    }

} // namespace Berserk
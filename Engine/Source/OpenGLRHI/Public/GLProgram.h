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

        GLShaderProgramBase() = default;

        ~GLShaderProgramBase() override = default;

        void setProgramData(GpuProgramData& data) override;

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

        void setTexture(const char *name, uint32 i) const override;

        void setSubroutines(EShaderType type, uint32 count, const char **functionNames) override;

        bool isLinked() const { return mResourceID != 0; }

    protected:

        template <typename RHITypeRef, typename GLShaderType>
        GLuint getShaderResourceID(const RHITypeRef& ref)
        {
            GLuint id = ((GLShaderType*)ref.pointer())->mResourceID;
            return id;
        }

        void addUniformVariable(GpuParamDesc& desc);

        void addUniformTexture(GpuParamDesc& desc);

        void addUniformBlock(GpuBlockDesc& desc);

        void addSubroutine(GpuSubroutineDesc& desc);

        GLuint getUniformVariable(const char* name) const;

        GLuint getUniformTexure(const char* name) const;

        GLuint getUniformBlock(const char* name) const;

        GLuint getSubroutine(const char* name) const;

    protected:

        static const int32 VALUE_NOT_FOUND = -1;
        static const uint32 SUBROUTINES_BUFFER_SIZE = 32;

        GLuint mResourceID = 0;
        GpuProgramData* mProgramData;

    };

    class GLShaderProgramVF final : public GLShaderProgramBase
    {
    public:

        GLShaderProgramVF(RHIVertexShaderRef vertexShader, RHIFragmentShaderRef fragmentShader, const char* debugName = "");

        ~GLShaderProgramVF() override;

    private:

        RHIVertexShaderRef mVertexShader;
        RHIFragmentShaderRef mFragmentShader;

    };

} // namespace Berserk

#endif //BERSERK_GLPROGRAM_H

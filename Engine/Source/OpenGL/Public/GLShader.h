//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLSHADER_H
#define BERSERK_GLSHADER_H

#include "Public/Platform/IShader.h"
#include "Public/Strings/StaticString.h"

namespace Berserk
{

    class GRAPHICS_API GLShader : public IShader
    {
    public:

        ~GLShader() = default;

        /** @copydoc IResource::initialize() */
        void initialize(const char* name) override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char* getName() override;

    public:

        void createProgram() override;

        void attachShader(ShaderType type, const char* source, const char* filename) override;

        void link() override;

        void validate() override;

        void use() override;

        void bindAttributeLocation(uint32 location, const char* name) override;

        void bindFragmentDataLocation(uint32 location, const char* name) override;

        void setUniform(const char* name, int32 i) override;

        void setUniform(const char* name, uint32 i) override;

        void setUniform(const char* name, float32 f) override;

        void setUniform(const char* name, const Vec2f& v) override;

        void setUniform(const char* name, const Vec3f& v) override;

        void setUniform(const char* name, const Vec4f& v) override;

        void setUniform(const char* name, const Mat2x2f& m) override;

        void setUniform(const char* name, const Mat3x3f& m) override;

        void setUniform(const char* name, const Mat4x4f& m) override;

        void setSubroutines(ShaderType type, uint32 count, uint32* indices) override;

        int32 getUniformLocation(const char* name) override;

        int32 getUniformBlockIndex(const char* name) override;

        int32 getAttributeLocation(const char* name) override;

        int32 getSubroutineLocation(ShaderType type, const char* name) override;

        uint32 getSubroutineIndex(ShaderType type, const char* name) override;

    public:

        static const int32 NOT_FOUND = -1;

        static uint32 getShaderType(ShaderType type);

    private:

        uint32 mProgram;
        uint32 mShaders[MAX_SHADER_COUNT];

        uint32 mReferenceCount;

        CName mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLSHADER_H
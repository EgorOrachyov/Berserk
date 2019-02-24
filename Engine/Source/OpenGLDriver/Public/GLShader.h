//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLSHADER_H
#define BERSERK_GLSHADER_H

#include "Containers/HashMap.h"
#include "Platform/IShader.h"
#include "Strings/StaticString.h"
#include "GLRenderDriver.h"

namespace Berserk
{

    class GRAPHICS_API GLShader : public IShader
    {
    public:

        GLShader() = default;

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

        void attachShader(IRenderDriver::ShaderType type, const char *source, const char *filename) override;

        void link() override;

        void validate() override;

        void use() override;

        void addUniformVariable(const char* name) override;

        void bindAttributeLocation(uint32 location, const char* name) override;

        void bindFragmentDataLocation(uint32 location, const char* name) override;

    public:

        void setUniform(const char* name, int32 i) override;

        void setUniform(const char* name, uint32 i) override;

        void setUniform(const char* name, float32 f) override;

        void setUniform(const char* name, const Vec2f& v) override;

        void setUniform(const char* name, const Vec3f& v) override;

        void setUniform(const char* name, const Vec4f& v) override;

        void setUniform(const char* name, const Mat2x2f& m) override;

        void setUniform(const char* name, const Mat3x3f& m) override;

        void setUniform(const char* name, const Mat4x4f& m) override;

        void setSubroutines(uint32 shaderType, uint32 count, uint32* indices) override;

    public:

        int32 getUniformLocation(const char* name) override;

        int32 getUniformBlockIndex(const char* name) override;

        int32 getAttributeLocation(const char* name) override;

        int32 getSubroutineLocation(uint32 shaderType, const char* name) override;

        uint32 getSubroutineIndex(uint32 shaderType, const char* name) override;

    public:

        static const int32 NOT_FOUND = -1;

    private:

        uint32 mProgram;                                        // Id of linked gpu program
        uint32 mReferenceCount;                                 // Reference count to this shader program
        uint32 mShaders[GLRenderDriver::MAX_SHADER_COUNT];      // Ids of shaders linked to the gpu program
        CName mResourceName;                                    // C-string name of resource
        HashMap<CName,uint32> mUniformMap;                      // Mapping of uniform variables to its locations

    };

} // namespace Berserk

#endif //BERSERK_GLSHADER_H
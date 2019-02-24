//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ISHADER_H
#define BERSERK_ISHADER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Math/MathInclude.h"
#include "Resource/IResource.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GRAPHICS_API IShader : public IResource
    {
    public:

        virtual void createProgram() = 0;

        virtual void attachShader(IRenderDriver::ShaderType shaderType, const char *source, const char *filename = "") = 0;

        virtual void link() = 0;

        virtual void validate() = 0;

        virtual void use() = 0;

        virtual void addUniformVariable(const char* name) = 0;

        virtual void bindAttributeLocation(uint32 location, const char* name) = 0;

        virtual void bindFragmentDataLocation(uint32 location, const char* name) = 0;

    public:

        virtual void setUniform(const char* name, int32 i) = 0;

        virtual void setUniform(const char* name, uint32 i) = 0;

        virtual void setUniform(const char* name, float32 f) = 0;

        virtual void setUniform(const char* name, const Vec2f& v) = 0;

        virtual void setUniform(const char* name, const Vec3f& v) = 0;

        virtual void setUniform(const char* name, const Vec4f& v) = 0;

        virtual void setUniform(const char* name, const Mat2x2f& m) = 0;

        virtual void setUniform(const char* name, const Mat3x3f& m) = 0;

        virtual void setUniform(const char* name, const Mat4x4f& m) = 0;

        virtual void setSubroutines(uint32 shaderType, uint32 count, uint32 *indices) = 0;

    public:

        virtual int32 getUniformLocation(const char* name) = 0;

        virtual int32 getUniformBlockIndex(const char* name) = 0;

        virtual int32 getAttributeLocation(const char* name) = 0;

        virtual int32 getSubroutineLocation(uint32 shaderType, const char* name) = 0;

        virtual uint32 getSubroutineIndex(uint32 shaderType, const char* name) = 0;

    };

} // namespace Berserk

#endif //BERSERK_ISHADER_H
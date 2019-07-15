//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHISHADERPROGRAM_H
#define BERSERK_RHISHADERPROGRAM_H

#include <RHIResource.h>
#include <RHIDefinitions.h>

namespace Berserk
{

    class GRAPHICS_API RHIShaderProgram : public RHIResource
    {
    public:

        virtual ~RHIShaderProgram() = default;

        /** Use this shader program as active */
        virtual void use() const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, int32 i) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, uint32 i) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, float32 f) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Vec2f& v) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Vec3f& v) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Vec4f& v) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Mat2x2f& m) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Mat3x3f& m) const = 0;

        /** Set uniform variable value via its name */
        virtual void setUniform(const char* name, const Mat4x4f& m) const = 0;

        /** Set all the shader subroutines via target shader and array of subroutine indices */
        virtual void setSubroutines(EShaderType type, uint32 count, const char* functionNames[]) = 0;

        /** @return Shader program name (for debug purposes) */
        virtual const String& getName() const = 0;

    };

    typedef TSharedPtr<RHIShaderProgram> RHIShaderProgramRef;

} // namespace Berserk

#endif //BERSERK_RHISHADERPROGRAM_H
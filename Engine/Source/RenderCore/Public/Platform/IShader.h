//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ISHADER_H
#define BERSERK_ISHADER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Math/MathInclude.h"
#include "Resource/IResource.h"
#include "Platform/IRenderDriver.h"
#include "Memory/Old/PoolAllocator.h"

namespace Berserk
{

    namespace Resources
    {

        /**
         * Abstract shader class to handle gpu shader program resource
         */
        class GRAPHICS_API IShader : public IResource
        {
        public:

            virtual ~IShader() = default;

            /** Creates gpu program object */
            virtual void createProgram(PoolAllocator *pool) = 0;

            /** Attach shader from source and type to the created program */
            virtual void attachShader(IRenderDriver::ShaderType shaderType, const char *source, const char *filename) = 0;

            /** Link compiled shaders to program */
            virtual void link() = 0;

            /** Validate program */
            virtual void validate() = 0;

            /** Set as active shader program */
            virtual void use() = 0;

            /** Specify uniform variable, which will be used in shader uniform calls */
            virtual void addUniformVariable(const char* name) = 0;

            /** Explicitly bind vertex shader attribute location */
            virtual void bindAttributeLocation(uint32 location, const char* name) = 0;

            /** Explicitly bind fragment shader data location */
            virtual void bindFragmentDataLocation(uint32 location, const char* name) = 0;

        public:

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, int32 i) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, uint32 i) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, float32 f) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Vec2f& v) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Vec3f& v) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Vec4f& v) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Mat2x2f& m) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Mat3x3f& m) = 0;

            /** Set uniform variable value via its name */
            virtual void setUniform(const char* name, const Mat4x4f& m) = 0;

            /** Set all the shader subroutines via target shader and array of subroutine indices */
            virtual void setSubroutines(uint32 shaderType, uint32 count, uint32 *indices) = 0;

            /** Set uniform block binding point */
            virtual void setUniformBlockBinding(const char* name, uint32 bindingPoint) = 0;

        public:

            /** @return Uniform variable location */
            virtual int32 getUniformLocation(const char* name) = 0;

            /** @return Uniform block index */
            virtual int32 getUniformBlockIndex(const char* name) = 0;

            /** @return Attribute location */
            virtual int32 getAttributeLocation(const char* name) = 0;

            /** @return Index of abstract subroutine */
            virtual int32 getSubroutineLocation(uint32 shaderType, const char* name) = 0;

            /** @return Location of concrete subroutine (function implementation) */
            virtual uint32 getSubroutineIndex(uint32 shaderType, const char* name) = 0;

            /** @return Memory used at gpu side */
            virtual uint32 getGPUMemoryUsage() = 0;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_ISHADER_H
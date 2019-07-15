//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHISHADER_H
#define BERSERK_RHISHADER_H

#include <RHIResource.h>

namespace Berserk
{

    /** Base shader linkable with shader program */
    class GRAPHICS_API RHIShader : public RHIResource
    {
    public:

        virtual ~RHIShader() = default;

        /** @return Shader source code */
        virtual const char* getSourceCode() const = 0;

    };

    class GRAPHICS_API RHIVertexShader : public RHIShader {};

    class GRAPHICS_API RHIFragmentShader : public RHIShader {};

    class GRAPHICS_API RHIGeometryShader : public RHIShader {};

    class GRAPHICS_API RHIComputeShader : public RHIShader {};

    class GRAPHICS_API RHITessControlShader : public RHIShader {};

    class GRAPHICS_API RHITessEvalShader : public RHIShader {};

} // namespace Berserk

#endif //BERSERK_RHISHADER_H
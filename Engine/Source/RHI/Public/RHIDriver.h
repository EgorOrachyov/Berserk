//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIDRIVER_H
#define BERSERK_RHIDRIVER_H

#include <RHIResource.h>
#include <RHIResources.h>

namespace Berserk
{

    class GRAPHICS_API RHIDriver
    {
    public:

        virtual ~RHIDriver() = default;

        virtual RHIVertexShaderRef createVertexShader(const char* code) = 0;

        virtual RHIFragmentShaderRef createFragmentShader(const char* code) = 0;

        virtual RHIGeometryShaderRef createGeometryShader(const char* code) = 0;

        virtual RHIComputeShaderRef createComputeShader(const char* code) = 0;

        virtual RHITessControlShaderRef createTessellationControlShader(const char* code) = 0;

        virtual RHITessEvalShaderRef createTessellationEvaluationShader(const char* code) = 0;

    };

} // namespace Berserk

#endif //BERSERK_RHIDRIVER_H
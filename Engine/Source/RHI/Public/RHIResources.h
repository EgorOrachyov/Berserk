//
// Created by Egor Orachyov on 2019-07-13.
//

#ifndef BERSERK_RHIRESOURCES_H
#define BERSERK_RHIRESOURCES_H

#include <RHIShader.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    typedef TSharedPtr<RHIVertexShader> RHIVertexShaderRef;

    typedef TSharedPtr<RHIFragmentShader> RHIFragmentShaderRef;

    typedef TSharedPtr<RHIGeometryShader> RHIGeometryShaderRef;

    typedef TSharedPtr<RHIComputeShader> RHIComputeShaderRef;

    typedef TSharedPtr<RHITessControlShader> RHITessControlShaderRef;

    typedef TSharedPtr<RHITessEvalShader> RHITessEvalShaderRef;

} // namespace Berserk

#endif //BERSERK_RHIRESOURCES_H

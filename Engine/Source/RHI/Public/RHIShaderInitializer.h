//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHISHADERINITIALIZER_H
#define BERSERK_RHISHADERINITIALIZER_H

#include <RHIDefinitions.h>
#include <Containers/TArray.h>
#include <Strings/String.h>

namespace Berserk
{
    struct GRAPHICS_API SubroutineInfo
    {
        /** Function name */
        String name;

        /** Actual shader location */
        EShaderType shaderType;
    };

    struct GRAPHICS_API UniformBlockInfo
    {
        /** Uniform block name */
        String name;

        /** Uniform block shader binding point */
        uint32 bindingPoint;
    }

    class GRAPHICS_API RHIShaderInitializer
    {
    public:

        explicit RHIShaderInitializer(IAllocator& allocator = Allocator::get())
        : uniformVarNames(allocator), uniformBlocksNames(allocator), subroutinesInfo(allocator) { }

        /** Name of variables to load [locations] into shader program table */
        TArray<String> uniformVarNames;

        /** To load uniform block bindings into shader program table */
        TArray<UniformBlockInfo> uniformBlocksNames;

        /** To load subroutine functions pointers info into shader tables */
        TArray<SubroutineInfo> subroutinesInfo;

    };

} // namespace Berserk

#endif //BERSERK_RHISHADERINITIALIZER_H
//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_SHADERINITIALIZER_H
#define BERSERK_SHADERINITIALIZER_H

#include <Containers/TArray.h>
#include <Strings/String.h>

namespace Berserk
{
    struct GRAPHICS_API SubroutineInfo
    {
        GENERATE_NEW_DELETE(SubroutineInfo);

        SubroutineInfo(const String& name, EShaderType shaderType)
            : name(name),
              shaderType(shaderType)
        {

        }

        /** Function name */
        String name;

        /** Actual shader location */
        EShaderType shaderType;
    };

    struct GRAPHICS_API UniformBlockInfo
    {
        GENERATE_NEW_DELETE(UniformBlockInfo);

        UniformBlockInfo()
        {}

        UniformBlockInfo(const String& name, uint32 bindingPoint)
            : name(name), bindingPoint(bindingPoint)
        {}

        /** Uniform block name */
        String name;

        /** Uniform block shader binding point */
        uint32 bindingPoint;
    };

    class GRAPHICS_API ShaderInitializer
    {
    public:

        explicit ShaderInitializer(IAllocator& allocator = Allocator::get())
        : uniformVarNames(allocator), uniformBlocksInfo(allocator), subroutinesInfo(allocator) { }

        /** Name of variables to load [locations] into shader program table */
        TArray<String> uniformVarNames;

        /** To load uniform block bindings into shader program table */
        TArray<UniformBlockInfo> uniformBlocksInfo;

        /** To load subroutine functions pointers info into shader tables */
        TArray<SubroutineInfo> subroutinesInfo;

    };

} // namespace Berserk

#endif //BERSERK_SHADERINITIALIZER_H
//
// Created by Egor Orachyov on 2019-08-27.
//

#ifndef BERSERK_GPUPROGRAMDATA_H
#define BERSERK_GPUPROGRAMDATA_H

#include <Misc/Types.h>
#include <Strings/String.h>
#include <Object/Allocatable.h>
#include <Rendering/Definitions.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /** Data for single GPU program param */
    struct ENGINE_API GpuParamDesc : public Allocatable
    {
        String name;
        uint32 index;
    };

    /** Data for single GPU program param block */
    struct ENGINE_API GpuBlockDesc : public Allocatable
    {
        String name;
        uint32 index;
        uint32 binding;
    };

    /** Data for single GPU program subroutine function */
    struct ENGINE_API GpuSubroutineDesc : public Allocatable
    {
        String name;
        uint32 index;
        EShaderType type;
    };

    /** Data about all the gpu program params (textures, variables and blocks) */
    struct ENGINE_API GpuProgramData : public Allocatable
    {
        explicit GpuProgramData(IAllocator& allocator = Allocator::get());

        TArray<GpuParamDesc> variables;
        TArray<GpuParamDesc> textures;
        TArray<GpuBlockDesc> blocks;
        TArray<GpuSubroutineDesc> subroutines;
    };

    typedef TSharedPtr<GpuProgramData> GpuProgramDataRef;

} // namespace Berserk

#endif //BERSERK_GPUPROGRAMDATA_H
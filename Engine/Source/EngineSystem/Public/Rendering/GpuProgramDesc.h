//
// Created by Egor Orachyov on 2019-08-27.
//

#ifndef BERSERK_GPUPROGRAMDESC_H
#define BERSERK_GPUPROGRAMDESC_H

#include <Containers/TArray.h>
#include <RHI/RHIDriver.h>
#include <Rendering/GpuProgramData.h>

namespace Berserk
{

    /** Single shader data */
    struct GpuShaderSource
    {
        explicit GpuShaderSource(IAllocator &allocator = Allocator::get()) : source(allocator) {}

        EShaderType type = EShaderType::ST_NotSupported;
        TArray<uint8> source;
    };

    /** Data to create single GPU program (primary loaded from internal engine file format) */
    struct GpuProgramDesc
    {
        explicit GpuProgramDesc(IAllocator &allocator = Allocator::get()) : shaders(allocator) {}

        uint32 flags = 0;
        String programName;
        GpuProgramDataRef programData;
        TArray<GpuShaderSource> shaders;
    };

} // namespace Berserk

#endif //BERSERK_GPUPROGRAMDESC_H
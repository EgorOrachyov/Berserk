//
// Created by Egor Orachyov on 2019-08-27.
//

#ifndef BERSERK_GPUPROGRAMMANAGER_H
#define BERSERK_GPUPROGRAMMANAGER_H

#include <RHI/RHIDriver.h>
#include <Rendering/GpuProgram.h>
#include <Rendering/GpuProgramImportData.h>

namespace Berserk
{

    /**
     * Manages all the GPU programs, created in the engine.
     * Allows to create programs (from raw source with) used on RHI side.
     */
    class ENGINE_API GpuProgramManager
    {
    public:

        /**
         * Initialize manager for single RHI driver kind
         * @param driver RHI driver to create actual RHI programs
         * @param allocator Memory allocator for internal usage
         */
        GpuProgramManager(RHIDriver &driver, IAllocator &allocator = Allocator::get());

        ~GpuProgramManager();

        const GpuProgramRef &createProgram(const GpuProgramImportData &importData);

        const GpuProgramRef &find(const String& name);

    private:

        IAllocator& mAllocator;
        RHIDriver& mDriver;
        TArray<GpuProgramRef> mPrograms;
        PoolAllocator mProgramsPool;

    };

} // namespace Berserk

#endif //BERSERK_GPUPROGRAMMANAGER_H
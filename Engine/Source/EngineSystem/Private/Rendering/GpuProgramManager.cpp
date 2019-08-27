//
// Created by Egor Orachyov on 2019-08-27.
//

#include "Rendering/GpuProgramManager.h"
#include <Logging/DebugLogMacros.h>
#include <Misc/ExceptionMacros.h>
#include <Misc/Assert.h>

namespace Berserk
{

    GpuProgramManager::GpuProgramManager(Berserk::RHIDriver &driver, Berserk::IAllocator &allocator)
        : mAllocator(allocator),
          mDriver(driver),
          mPrograms(allocator),
          mProgramsPool(sizeof(GpuProgram), PoolAllocator::DEFAULT_CHUNK_COUNT, allocator)
    {

    }

    GpuProgramManager::~GpuProgramManager()
    {
        DEBUG_LOG_DISPLAY("GpuProgramManager: destroy");
    }

    const GpuProgramRef& GpuProgramManager::createProgram(const GpuProgramImportData &importData)
    {
        const GpuProgramRef* found = findPtr(importData.programName);

        if (found)
        {
            return *found;
        }

        uint32 flags = importData.flags;
        const String& name = importData.programName;
        const GpuProgramDataRef& programData = importData.programData;

        RHIVertexShaderRef vertexShader;
        RHIFragmentShaderRef fragmentShader;
        RHIShaderProgramRef programRHI;

        if (flags & ST_Vertex)
        {
            GpuShaderSource* data = importData.shaders.find([](const GpuShaderSource& s) { return s.type == ST_Vertex; });
            vertexShader = mDriver.createVertexShader((char*) data->source.getRawBuffer());

            assertion_msg(vertexShader.isPresent(), "GpuProgramManager: cannot load vertex shader [name: %s]", name.get());
        }

        if (flags & ST_Fragment)
        {
            GpuShaderSource* data = importData.shaders.find([](const GpuShaderSource& s) { return s.type == ST_Fragment; });
            fragmentShader = mDriver.createFragmentShader((char*) data->source.getRawBuffer());

            assertion_msg(vertexShader.isPresent(), "GpuProgramManager: cannot load fragment shader [name: %s]", name.get());
        }

        switch (flags)
        {
            case ST_Vertex | ST_Fragment:
                // programRHI = mDriver.createShaderProgram(vertexShader, fragmentShader);
                // assertion_msg(programRHI.isPresent(), "GpuProgramManager: cannot create program [name: %s]", name.get());
            default:
                engine_exception("GpuProgramManager: unsupported flags [name: %s]", name.get());
        }

        auto gpuProgramPtr = mProgramsPool.engine_new_const<GpuProgram>(
                name,
                programData,
                programRHI);

        return mPrograms.emplace(gpuProgramPtr, &mProgramsPool);
    }

    const GpuProgramRef& GpuProgramManager::find(const Berserk::String &name) const
    {
        const GpuProgramRef* program = findPtr(name);

        if (program)
        {
            return *program;
        }
        else
        {
            engine_exception("GpuProgramManager: cannot find program [name: %s]", name.get());
        }
    }

    const GpuProgramRef* GpuProgramManager::findPtr(const Berserk::String &name) const
    {
        auto predicate = [&](const GpuProgramRef& program) -> bool
        {
            return program->getProgramName() == name;
        };

        return mPrograms.findf(predicate);
    }

} // namespace Berserk
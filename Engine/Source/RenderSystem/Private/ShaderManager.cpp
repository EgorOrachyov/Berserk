//
// Created by Egor Orachyov on 2019-08-16.
//

#include "ShaderManager.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    ShaderManager::ShaderManager(Berserk::IShaderImporter &importer, Berserk::RHIDriverRef driver,
                                 Berserk::IAllocator &allocator)
         : mAllocator(allocator),
           mShaderImporter(importer),
           mMapPool(NameShaderMap::getNodeSize(), PoolAllocator::DEFAULT_CHUNK_COUNT, allocator),
           mShadersMap(allocator, mMapPool),
           mDriver(std::move(driver))
    {

    }

    RHIShaderRef ShaderManager::load(const char* shadername, const char *filename)
    {
        RHIShaderRef* shader = mShadersMap.get(Wrapper(shadername));

        if (shader)
        {
            return *shader;
        }

        TSharedPtr<ShaderImportData> data = mShaderImporter.import(filename);

        uint32 flags = data->getShaderFlags();

        switch (flags)
        {
            case ST_Vertex | ST_Fragment:
                shader = loadVFshader_internal(data);
                break;

            default:
                DEBUG_LOG_ERROR("ShaderManager: unknown shaders type flags [name: %s]", shadername);
                return RHIShaderRef();
        }

        if (!shader)
        {
            DEBUG_LOG_ERROR("ShaderManager: cannot load shader program [name: %s]", shadername);
            return RHIShaderRef();
        }

        return *shader;
    }

    ShaderManager::~ShaderManager()
    {
        DEBUG_LOG_DISPLAY("ShaderManager: destroy");
    }

    RHIShaderRef* ShaderManager::loadVFshader_internal(const Berserk::TSharedPtr<Berserk::ShaderImportData> &data)
    {

    }

} // namespace Berserk
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
        mShadersMap.setHashFunction(String::hash);
    }

    RHIShaderProgramRef ShaderManager::load(const char* shadername, const char *filename)
    {
        RHIShaderProgramRef* shader = mShadersMap.get(Wrapper(shadername));

        if (shader)
        {
            DEBUG_LOG_DISPLAY("ShaderManager: find shader [name: %s]", shadername);
            return *shader;
        }

        TSharedPtr<ShaderImportData> data = mShaderImporter.import(filename);

        uint32 flags = data->getShaderFlags();

        switch (flags)
        {
            case ST_Vertex | ST_Fragment:
                shader = loadShaderVF_internal(data);
                break;

            default:
                DEBUG_LOG_ERROR("ShaderManager: unknown shaders type flags [name: %s]", shadername);
                return RHIShaderProgramRef();
        }

        if (shader == nullptr)
        {
            DEBUG_LOG_ERROR("ShaderManager: cannot load shader program [name: %s]", shadername);
            return RHIShaderProgramRef();
        }

        return *shader;
    }

    ShaderManager::~ShaderManager()
    {
        DEBUG_LOG_DISPLAY("ShaderManager: destroy");
    }

    RHIShaderProgramRef* ShaderManager::loadShaderVF_internal(const TSharedPtr<ShaderImportData> &data)
    {
        ShaderData* vertexShaderData = data->getShadersData().find([](const ShaderData& s) {
            return s.getShaderType() == EShaderType::ST_Vertex;
        });

        ShaderData* fragmentShaderData = data->getShadersData().find([](const ShaderData& s) {
            return s.getShaderType() == EShaderType::ST_Fragment;
        });

        if (vertexShaderData == nullptr || fragmentShaderData == nullptr)
        {
            DEBUG_LOG_ERROR("ShaderManager: no shader data from flags [name: %s]",
                    data->getShaderName().get());

            return nullptr;
        }

        RHIVertexShaderRef vertexShader;
        RHIShaderProgramRef shaderProgram;
        RHIFragmentShaderRef fragmentShader;

        try {
            vertexShader = mDriver->createVertexShader(
                    vertexShaderData->getSourceCode());

            fragmentShader = mDriver->createFragmentShader(
                    fragmentShaderData->getSourceCode());
        }
        catch (Exception& e) {
            DEBUG_LOG_ERROR("ShaderManager: cannot create shader [name: %s]", data->getShaderName().get());
            return nullptr;
        }

        if (vertexShader.isNull() || fragmentShader.isNull())
            return nullptr;

        try {
            shaderProgram = mDriver->createShaderProgram(
                    vertexShader,
                    fragmentShader,
                    data->getShaderInitializer());
        }
        catch (Exception& e) {
            return nullptr;
        }

        if (shaderProgram.isNull())
            return nullptr;

        RHIShaderProgramRef& program = mShadersMap.put(data->getShaderName(), shaderProgram);

        DEBUG_LOG_DISPLAY("ShaderManager: load shader program [name: %s]",
                data->getShaderName().get());

        return &program;
    }

} // namespace Berserk
//
// Created by Egor Orachyov on 2019-07-23.
//

#include "Importers/XMLShaderImporter.h"
#include <XMLDocument.h>
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    EShaderPlatform platformFromString(const char* platform)
    {
        Wrapper str = platform;

        if (str == "OpenGL")
        {
            return SP_OpenGL;
        }
        else if (str == "DirectX")
        {
            return SP_DirectX;
        }
        else
        {
            return SP_NotSupported;
        }
    }

    EShaderType typeFromString(const char* type)
    {
        Wrapper str = type;

        if (str == "Vertex")
        {
            return ST_Vertex;
        }
        else if (str == "Fragment")
        {
            return ST_Fragment;
        }
        else if (str == "Geometry")
        {
            return ST_Geometry;
        }
        else if (str == "Compute")
        {
            return ST_Compute;
        }
        else if (str == "TessControl")
        {
            return ST_TessellationControl;
        }
        else if (str == "TessEval")
        {
            return ST_TessellationEvaluation;
        }
        else
        {
            return ST_NotSupported;
        }

    }

    XMLShaderImporter::XMLShaderImporter(Berserk::EShaderPlatform platform, IAllocator& allocator)
        : mAllocator(allocator),
          mPlatform(platform)
    {

    }

    EShaderPlatform XMLShaderImporter::getPlatform() const
    {
        return mPlatform;
    }

    TSharedPtr<ShaderImportData> XMLShaderImporter::import(const char *filename)
    {
        XMLDocument document(filename);
        XMLNode program = document.getFirst();
        XMLNode driver = program.getChild();

        while (!driver.isEmpty() && platformFromString(driver.getAttribute("type").getValue()) != mPlatform)
        {
            driver = driver.getNext();
        }

        if (driver.isEmpty())
        {
            return TSharedPtr<ShaderImportData>();
        }

        String shaderName = program.getAttribute("name").getValue();

        for (auto node = driver.getChild(); !node.isEmpty(); node = node.getNext())
        {
            if (Wrapper("shaders") == node.getName())
            {

            }
            else if (Wrapper("uniforms") == node.getName())
            {

            }
            else if (Wrapper("blocks") == node.getName())
            {

            }
            else if (Wrapper("subroutine") == node.getName())
            {

            }
            else
            {
                DEBUG_LOG_WARNING("XMLShaderImporter: unknown node type");
            }
        }

        return TSharedPtr<ShaderImportData>();
    }

} // namespace Berserk
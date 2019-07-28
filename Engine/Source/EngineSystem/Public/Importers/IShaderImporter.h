//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_ISHADERIMPORTER_H
#define BERSERK_ISHADERIMPORTER_H

#include <Resource/TSharedPtr.h>
#include <Rendering/ShaderImportData.h>

namespace Berserk
{

    /** Shader importer, used by render system to load target RHI platform shaders */
    class RENDER_API IShaderImporter
    {
    public:

        virtual ~IShaderImporter() = default;

        /**
         * Get shader target platform, which defines what shader data is imported
         * @return Shader platform enum
         */
        virtual EShaderPlatform getPlatform() const = 0;

        /**
         * Import single shader from source file
         * @param filename Full file name/path with shader data
         * @return Pointer to loaded shader data, or null if there is some kind of error
         */
        virtual TSharedPtr<ShaderImportData> import(const char* filename) = 0;

    };

} // namespace Berserk

#endif //BERSERK_ISHADERIMPORTER_H
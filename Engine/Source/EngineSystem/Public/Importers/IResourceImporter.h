//
// Created by Egor Orachyov on 2019-08-26.
//

#ifndef BERSERK_IRESOURCEIMPORTER_H
#define BERSERK_IRESOURCEIMPORTER_H

#include <Resource/Resource.h>
#include "ImportOptions.h"

namespace Berserk
{

    /**
     * Interface for importer of the single kind of the resource
     * @note Importer primary used for importing resources from no-engine
     *       formats. Should be used only for resource pre-processing or debug.
     */
    class ENGINE_API IResourceImporter : public Allocatable
    {
    public:

        virtual ~IResourceImporter() = default;

        /**
         * Checks whether this import supports loading resource from specified extension
         * @param extension File extension to check
         * @return True, if can import resource from file with extension
         */
        virtual bool isExtensionSupported(const char* extension) const = 0;

        /**
         * Import ready-to-use resource (fully loads)
         * @param path Full OS file name with path and extension
         * @param options Resource import options used for loading
         * @return Loaded resource (or null if some error occurs)
         */
        virtual TSharedPtr<Resource> import(const String& path, const ImportOptions& options) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCEIMPORTER_H
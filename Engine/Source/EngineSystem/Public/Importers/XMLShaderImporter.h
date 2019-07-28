//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_XMLSHADERIMPORTER_H
#define BERSERK_XMLSHADERIMPORTER_H

#include <Importers/IShaderImporter.h>

namespace Berserk
{

    /**
     * XML format files based shader importer:
     * only imports shader program data
     *
     * @note Thread-Safe
     */
    class RENDER_API XMLShaderImporter : public IShaderImporter
    {
    public:

        /**
         * Initialize shader importer for specified targedt platform
         * @param platform Target render driver platform (default: SP_OpenGL)
         */
        explicit XMLShaderImporter(EShaderPlatform platform = SP_OpenGL, IAllocator& allocator = Allocator::get());

        ~XMLShaderImporter() override = default;

        EShaderPlatform getPlatform() const override;

        TSharedPtr<ShaderImportData> import(const char *filename) override;

    private:

        IAllocator& mAllocator;
        EShaderPlatform mPlatform;


    };

} // namespace Berserk

#endif //BERSERK_XMLSHADERIMPORTER_H
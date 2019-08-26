//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_XMLSHADERDATAIMPORTER_H
#define BERSERK_XMLSHADERDATAIMPORTER_H

#include <Importers/IShaderDataImporter.h>

namespace Berserk
{

    /**
     * XML format files based shader importer: only imports shader program data
     */
    class RENDER_API XMLShaderDataImporter : public IShaderDataImporter
    {
    public:

        /**
         * Initialize shader importer for specified target platform
         * @param platform Target render driver platform (default: SP_OpenGL)
         */
        explicit XMLShaderDataImporter(EShaderPlatform platform = SP_OpenGL, IAllocator& allocator = Allocator::get());

        ~XMLShaderDataImporter() override = default;

        EShaderPlatform getPlatform() const override;

        TSharedPtr<ShaderImportData> import(const char *filename) override;

    private:

        IAllocator& mAllocator;
        EShaderPlatform mPlatform;

    };

} // namespace Berserk

#endif //BERSERK_XMLSHADERDATAIMPORTER_H
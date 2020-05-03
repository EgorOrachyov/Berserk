/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IMAGEIMPORTER_H
#define BERSERK_IMAGEIMPORTER_H

#include <Image.h>

namespace Berserk {

    class ImageImporter: public IResourceImporter {
    public:
        ImageImporter();
        ~ImageImporter();

        /** @copydoc IResourceImporter::import() */
        EError import(TPtrShared<Resource> &resource, const CString &importPath, const TPtrShared<IResourceImportOptions> &options) override;

        /** @copydoc IResourceImporter::getRecognizedExtensions() */
        const TArray<CString> &getRecognizedExtensions() const override;

        /** @copydoc IResourceImporter::getImporterName() */
        const char *getImporterName() const override;

        /** @copydoc IResourceImporter::getImporterProjectName() */
        const char *getImporterProjectName() override;

    private:
        TArray<CString> mRecognizedExtensions;
    };

}

#endif //BERSERK_IMAGEIMPORTER_H
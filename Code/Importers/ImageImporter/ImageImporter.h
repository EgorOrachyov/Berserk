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
#include <TRef.h>

namespace Berserk {

    /** STB image base image importer */
    class ImageImporter: public ResourceImporter {
    public:
        ImageImporter();
        ~ImageImporter();

        /** @copydoc ResourceImporter::import() */
        EError import(TPtrShared<Resource> &resource, const CString &importPath, TRef<const ResourceImportOptions> options) override;

        /** @copydoc ResourceImporter::getRecognizedExtensions() */
        const TArray<CString> &getRecognizedExtensions() const override;

        /** @copydoc ResourceImporter::getImporterName() */
        const char *getImporterName() const override;

        /** @copydoc ResourceImporter::getImporterProjectName() */
        const char *getImporterProjectName() override;

        /** @copydoc ResourceImporter::threadSafe() */
        bool threadSafe() const override;
        
    private:

        /** Imported resource files extension */
        TArray<CString> mRecognizedExtensions;

        /** For thread-safe access */
        mutable Mutex mAccessMutex;
    };

}

#endif //BERSERK_IMAGEIMPORTER_H
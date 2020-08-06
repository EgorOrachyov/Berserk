/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FONTIMPORTER_H
#define BERSERK_FONTIMPORTER_H

#include <Font.h>
#include <Containers/BinaryData.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Berserk {

    /** Importer for True-Type Fonts (freetype2 based) */
    class FontImporter : public ResourceImporter {
    public:

        FontImporter();
        ~FontImporter();

        /** @copydoc ResourceImporter::import() */
        EError import(TPtrShared<Resource> &resource, const CString &importPath, const TPtrShared<ResourceImportOptions> &options) override;

        /** @copydoc ResourceImporter::getRecognizedExtensions() */
        const TArray<CString> &getRecognizedExtensions() const override;

        /** @copydoc ResourceImporter::getImporterName() */
        const char *getImporterName() const override;

        /** @copydoc ResourceImporter::getImporterProjectName() */
        const char *getImporterProjectName() override;

        /** @copydoc ResourceImporter::threadSafe() */
        bool threadSafe() const override;

    private:
        static const uint32 FREETYPE_SIZE_SHIFT = 6;
        static const uint32 BITMAP_GLYPH_OFFSET = 2;
        static const uint32 BITMAP_GLYPHS_IN_ROW = 20;

        /** Store rendered bitmaps for glyphs */
        BinaryData mSource;

        /** Data for single image bitmap with all glyphs */
        BinaryData mBitmap;

        /** Imported resource files extension */
        TArray<CString> mRecognizedExtensions;

        /** For thread-safe access */
        mutable Mutex mAccessMutex;

        /** Freetype library instance handler */
        FT_Library mLibrary = nullptr;

    };

}

#endif //BERSERK_FONTIMPORTER_H
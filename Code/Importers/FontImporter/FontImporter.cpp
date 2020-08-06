/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <FontImporter/FontImporter.h>
#include <LogMacro.h>

namespace Berserk {

    void getAsciiCharset(const wchar* &array, uint32& size) {
        static const int32 count = 0x7E - 0x20 + 1 + 1;
        static wchar codepoints[count];

        codepoints[0] = 0;
        for (int32 i = 0; i < count - 1; i++) {
            codepoints[i+1] = 0x20 + i;
        }

        array = codepoints;
        size = count;
    }

    FontImporter::FontImporter() {
        mRecognizedExtensions.ensureToAdd(1);
        mRecognizedExtensions.emplace("ttf");

        auto error = FT_Init_FreeType(&mLibrary);
        BERSERK_COND_ERROR_RET(!error, "Failed to initialize freetype library");

        registerImporter();
        BERSERK_LOG_INFO("Initialize FontImporter (freetype2)");
    }

    FontImporter::~FontImporter() {
        if (!mLibrary)
            return;

        FT_Done_FreeType(mLibrary);
        mLibrary = nullptr;

        unregisteImporter();
        BERSERK_LOG_INFO("Finalize FontImporter (freetype2)");
    }

    EError FontImporter::import(TPtrShared<Resource> &resource, const CString &importPath, const TPtrShared<ResourceImportOptions> &options) {
        Guard scopedLock(mAccessMutex);

        const FontImportOptions* fontOptions = Font::getDefaultImportOptions().getPtr();

        if (options.isNotNull()) {
            auto optionsPtr = options.getPtr();
            auto fontOptionsPtr = dynamic_cast<FontImportOptions*>(optionsPtr);

            BERSERK_COND_ERROR(fontOptionsPtr, "Provided invalid import options for font: %s", importPath.data());

            if (fontOptionsPtr)
                fontOptions = fontOptionsPtr;
        }

        auto fontSize = fontOptions->getFontSize();
        auto dynamicWidth = fontOptions->getIsAutoWidth();
        auto ascii = fontOptions->getIsAsciiCharSet();
        BERSERK_COND_ERROR_RET_VALUE(EError::FAILED_IMPORT_RESOURCE, ascii, "Supported only ascii character set: %s", importPath.data());

        if (dynamicWidth) {
            // Tell freetype to dynamically eval glyphs width
            fontSize.width() = 0;
        }

        FT_Error error;
        FT_Face face = nullptr;

        error = FT_New_Face(mLibrary, importPath.data(), 0, &face);
        BERSERK_COND_ERROR_RET_VALUE(EError::FAILED_IMPORT_RESOURCE, !error, "Failed to load font face: %s", importPath.data());

        FT_Select_Charmap(face, ft_encoding_unicode);
        FT_Set_Pixel_Sizes(face, fontSize.width(), fontSize.height());

        const wchar* codepoints;
        uint32 count;

        if (ascii) {
            // todo: support arbitrary char set
            getAsciiCharset(codepoints, count);
        }

        CString fontName;
        Font::Glyphs glyphs;
        Font::GlyphsIdx glyphsIdx;

        glyphs.ensureToAdd(count);

        {
            auto fontNameStr = CStringUtility::findLast(importPath.data(), "/");
            if (fontNameStr == nullptr) fontNameStr = importPath.data();

            CString fileName = fontNameStr + 1;
            fontName = fileName.removeExtension();
        }

        int32 glyphsLoaded = 0;
        int32 sourceHeight = 0;
        int32 sourceWidth = 0;

        mSource.clear();
        mBitmap.clear();

        for (uint32 i = 0; i < count; i++) {
            wchar codepoint = codepoints[i];

            error = FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
            if (error) continue;

            FT_GlyphSlot g = face->glyph;

            FontGlyph glyph;
            glyph.codepoint = codepoint;
            glyph.size = Size2i(g->bitmap.width, g->bitmap.rows);
            glyph.bearing = Size2i(g->bitmap_left, g->bitmap_top);
            glyph.advance = Size2i(g->advance.x >> FREETYPE_SIZE_SHIFT, g->advance.y >> FREETYPE_SIZE_SHIFT);

            glyphsIdx.add(codepoint, glyphs.size());
            glyphs.add(glyph);

            mSource.add(g->bitmap.buffer, glyph.size[0] * glyph.size[1]);

            glyphsLoaded += 1;
            sourceWidth = Math::max(sourceWidth, glyph.size[0]);
            sourceHeight = Math::max(sourceHeight, glyph.size[1]);
        }

        int32 bitmapCols = BITMAP_GLYPHS_IN_ROW;
        int32 bitmapRows = glyphsLoaded / bitmapCols + (glyphsLoaded % bitmapCols? 1 : 0);

        int32 bitmapOffsetsHz = bitmapCols - 1;
        int32 bitmapOffsetsVt = (bitmapRows > 1? bitmapRows - 1: 0);

        int32 bitmapWidth = bitmapCols * sourceWidth + bitmapOffsetsHz * BITMAP_GLYPH_OFFSET;
        int32 bitmapHeight = bitmapRows * sourceHeight + bitmapOffsetsVt * BITMAP_GLYPH_OFFSET;
        int32 bitmapSize = bitmapWidth * bitmapHeight;

        mBitmap.resize(bitmapSize);
        uint8* destination = mBitmap.data();
        const uint8* source = mSource.data();

        Memory::set(destination, 0x0, bitmapSize);

        uint32 readOffset = 0;

        for (uint32 i = 0; i < glyphs.size(); i++) {
            int32 r = i / bitmapCols;
            int32 c = i % bitmapCols;

            int32 writeOffset = r * (sourceHeight + BITMAP_GLYPH_OFFSET) * bitmapWidth + c * (sourceWidth + BITMAP_GLYPH_OFFSET);

            auto& glyph = glyphs[i];

            uint32 width = glyph.size[0];
            uint32 rows = glyph.size[1];

            for (uint32 line = 0; line < rows; line++) {
                Memory::copy(destination + writeOffset, source + readOffset, width);

                writeOffset += bitmapWidth;
                readOffset += width;
            }
        }

        TPtrShared<Image> bitmap = TPtrShared<Image>::make();
        bitmap->setName(fontName + "_bitmap");
        bitmap->create(bitmapWidth, bitmapHeight, EPixelFormat::R8, destination, false);

        TPtrShared<Font> font = TPtrShared<Font>::make();
        font->create(Size2i(sourceWidth,sourceHeight), fontName, std::move(glyphs), std::move(glyphsIdx), std::move(bitmap));
        font->setName(fontName + "_font");
        font->setImportPath(importPath);
        font->setImportTime(TimeValue::nowAsTime());
        font->setUUID(UUID::generate());

        resource = (TPtrShared<Resource>) font;
    }

    const TArray<CString> &FontImporter::getRecognizedExtensions() const {
        return mRecognizedExtensions;
    }

    const char *FontImporter::getImporterName() const {
        return "FontImporter";
    }

    const char *FontImporter::getImporterProjectName() {
        return "BerserkImporters";
    }

    bool FontImporter::threadSafe() const {
        return true;
    }

}

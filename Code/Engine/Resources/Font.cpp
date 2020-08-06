/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Font.h>

namespace Berserk {

    bool Font::isLoaded() const {
        return mBitmap.isNotNull();
    }

    void Font::create(Size2i size, CString name, Font::Glyphs glyphs, Font::GlyphsIdx glyphsIdx, TPtrShared<Image> bitmap) {
        mFontSize = std::move(size);
        mFontName = std::move(name);
        mGlyphs = std::move(glyphs);
        mGlyphsIdx = std::move(glyphsIdx);
        mBitmap = std::move(bitmap);
    }

    Size2i Font::getTextSize(const wchar *text) const {
        Size2i size = {0,0};
        FontGlyph glyph;

        while (*text != L'\0') {
            auto found = getGlyph(*text, glyph);

            if (found) {
                size[0] += glyph.advance[0];
                size[1] = Math::max(size[1], glyph.size[1]);
            }

            text += 1;
        }

        return size;
    }

    Size2i Font::getTextSize(const char *text) const {
        Size2i size = {0,0};
        FontGlyph glyph;

        wchar codepoint[10];

        while (*text != '\0') {
            std::mbstowcs(codepoint, text, 1);
            auto found = getGlyph(*codepoint, glyph);

            if (found) {
                size[0] += glyph.advance[0];
                size[1] = Math::max(size[1], glyph.size[1]);
            }

            text += 1;
        }

        return size;
    }

    Size2i Font::getTextSize(const WString &text) const {
        return Font::getTextSize(text.data());
    }

    Size2i Font::getTextSize(const CString &text) const {
        return Font::getTextSize(text.data());
    }


    bool Font::getGlyph(wchar codepoint, FontGlyph &glyph) const {
        auto found = mGlyphsIdx.getPtr(codepoint);

        if (found.isNotNull()) {
            glyph = mGlyphs[*found];
            return true;
        }

        return false;
    }

    const TPtrShared<FontImportOptions>& Font::getDefaultImportOptions() {
        static TPtrShared<FontImportOptions> importOptions = TPtrShared<FontImportOptions>::make();
        return importOptions;
    }

    void Font::showDebugInfo() {
        Resource::showDebugInfo();
        printf(" Type: Font\n");
        printf(" Font name: %s\n", mFontName.data());
        printf(" Font size: %ix%ipx\n", mFontSize[0], mFontSize[1]);
        printf(" Glyphs count: %u\n", mGlyphs.size());
        printf(" Bitmap size: %ux%upx\n", mBitmap->getWidth(), mBitmap->getHeight());
    }

}

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Resources/Font.h>

namespace Berserk {

    bool Font::isLoaded() const {
        for (auto& bitmap: mBitmaps) {
            if (!bitmap->isLoaded())
                return false;
        }

        return true;
    }

    bool Font::getGlyph(wchar codepoint, Font::Glyph &glyph) const {
        auto found = mGlyphsIdx.getPtr(codepoint);
        if (found.isNotNull()) {
            glyph = mGlyphs[*found];
            return true;
        }

        return false;
    }

    Size2i Font::getTextSize(const wchar *text) const {
        Size2i size = {0,0};
        Glyph glyph;

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
        Glyph glyph;

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

    const TPtrShared<FontImportOptions>& Font::getDefaultImportOptions() {
        static TPtrShared<FontImportOptions> importOptions = TPtrShared<FontImportOptions>::make();
        return importOptions;
    }

}

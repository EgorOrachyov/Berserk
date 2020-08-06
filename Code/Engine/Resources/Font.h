/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FONT_H
#define BERSERK_FONT_H

#include <String/WString.h>
#include <String/CString.h>
#include <Containers/TArray.h>
#include <Containers/TMap.h>
#include <FontImportOptions.h>
#include <FontGlyph.h>
#include <Resource.h>
#include <Image.h>

namespace Berserk {

    /**
     * @brief Font resource
     *
     * Represents named font resource of the fixed single size in pixel,
     * which is imported to the engine from TTF fonts format.
     *
     * Stores glyphs bitmap data in the single packed atlas image.
     */
    class Font : public Resource {
    public:
        /** Typedefs for common containers */
        using Glyphs = TArray<FontGlyph>;
        using GlyphsIdx = TMap<wchar,uint32,THashRaw<wchar>>;

        Font() = default;
        Font(const Font& other) = default;

        ~Font() override = default;

        /** Create from with specified properties and data */
        void create(Size2i size, CString name, Glyphs glyphs, GlyphsIdx glyphsIdx, TPtrShared<Image> bitmap);

        /** @return Text size for specified string in this font*/
        Size2i getTextSize(const wchar* text) const;

        /** @return Text size for specified string in this font*/
        Size2i getTextSize(const char* text) const;

        /** @return Text size for specified string in this font*/
        Size2i getTextSize(const WString &text) const;

        /** @return Text size for specified string in this font*/
        Size2i getTextSize(const CString &text) const;

        /** @return Attempts to get glyph info (true if has glyph for the code point)*/
        bool getGlyph(wchar codepoint, FontGlyph& glyph) const;

        /** @return Loaded font name  */
        const CString &getFontName() const { return mFontName; }

        /** @return Font size in pixels */
        const Size2i &getFontSize() const { return mFontSize; }

        /** @return Bitmap with glyph pixels */
        const TPtrShared<Image> &getBitmap() const { return mBitmap; }

        /** @return Default import options */
        static TPtrShared<FontImportOptions> &getDefaultImportOptions();

    private:

        /** Map codepoint to the glyph index */
        GlyphsIdx mGlyphsIdx;

        /** Font human readable name (may be not unique) */
        CString mFontName;

        /** Actual glyphs data */
        Glyphs mGlyphs;

        /** Bitmap with glyph pixels */
        TPtrShared<Image> mBitmap;

        /** Font size in px */
        Size2i mFontSize;

    };

}

#endif //BERSERK_FONT_H
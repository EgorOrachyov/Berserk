/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FONT_H
#define BERSERK_FONT_H

#include <Resources/Texture.h>
#include <String/WString.h>
#include <String/CString.h>
#include <Containers/TArray.h>
#include <Containers/TMap.h>

namespace Berserk {

    class FontImportOptions : public IResourceImportOptions {
    public:
        void setFontName(CString name) { fontName = std::move(name); }
        void setFontSize(const Size2i &size) { fontSize = size; }
        const CString& getFontName() const { return fontName; }
        const Size2i& getFontSize() const { return fontSize; }
        bool getAutoWidth() const { return autoWidth; }
    private:
        CString fontName;
        Size2i fontSize = { 10, 10 };
        bool autoWidth = true;
    };

    class Font : public Resource {
    public:

        /** Represent single printable character */
        struct Glyph {
            int32 bitmapIndex = -1;
            /** Character glyph width and height in pixels */
            Size2i size;
            /** Offset from baseline to left of glyph and to top of glyph in pixels */
            Size2i bearing;
            /** Offset to advance to next glyph in X and Y axis in pixels */
            Size2i advance;
            /** Left/Bottom, Top/Right glyph corners positions in font texture */
            Vec4f textureCoords;

            /** For font serialization */
            friend Archive& operator<<(Archive& archive, const Glyph& glyph) {
                archive << glyph.bitmapIndex;
                archive << glyph.size;
                archive << glyph.bearing;
                archive << glyph.advance;
                archive << glyph.textureCoords;

                return archive;
            }

            /** For font deserialization */
            friend Archive& operator>>(Archive& archive, Glyph& glyph) {
                archive >> glyph.bitmapIndex;
                archive >> glyph.size;
                archive >> glyph.bearing;
                archive >> glyph.advance;
                archive >> glyph.textureCoords;

                return archive;
            }
        };

        bool isLoaded() const override;

        Size2i getTextSize(const wchar* text) const;
        Size2i getTextSize(const char* text) const;
        Size2i getTextSize(const WString &text) const;
        Size2i getTextSize(const CString &text) const;

        bool getGlyph(wchar codepoint, Glyph& glyph) const;
        const CString &getFontName() const { return mFontName; }
        const Size2i &getFontSize() const { return mFontSize; }

        static const TPtrShared<FontImportOptions> &getDefaultImportOptions();

    private:
        /** Font human readable name (must be not unique) */
        CString mFontName;

        /** Font size in px */
        Size2i mFontSize;

        /** Map codepoint to the glyph index */
        TMap<wchar,uint32,THashRaw<wchar>> mGlyphsIdx;

        /** Actual glyphs data */
        TArray<Glyph> mGlyphs;

        /** Bitmaps for font rendering */
        TArray<TPtrShared<Texture2D>> mBitmaps;
    };

}

#endif //BERSERK_FONT_H
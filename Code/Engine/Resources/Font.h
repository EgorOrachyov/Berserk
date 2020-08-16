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
#include <Resources/FontImportOptions.h>
#include <Resources/FontGlyph.h>
#include <Resource.h>
#include <Image.h>
#include <Reflection/Object.h>
#include <RenderResources/GpuFont.h>

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
        BERSERK_CLASS(Font,Resource)

    public:
        /** Typedefs for common containers */
        using Glyphs = Render::GpuFont::Glyphs;
        using GlyphsIdx = Render::GpuFont::GlyphsIdx;

        /** Printed glyph when failed to find required glyphs */
        static const wchar NULL_GLYPH = Render::GpuFont::NULL_GLYPH;

        /** Create from specified properties and data */
        Font(Size2i size, CString name, Glyphs glyphs, GlyphsIdx glyphsIdx, TPtrShared<Image> bitmap);

        ~Font() override = default;

        /** @return True if font loaded and ready for usage */
        bool isLoaded() const override;

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

        /** @return Glyphs indices by codepoint */
        const Font::GlyphsIdx& getGlyphsIdx() const { return mGlyphsIdx; }

        /** @return Array of glyphs */
        const Font::Glyphs& getGlyphs() const { return mGlyphs; }

        /** @return Loaded font name  */
        const CString &getFontName() const { return mFontName; }

        /** @return Font size in pixels */
        const Size2i &getFontSize() const { return mFontSize; }

        /** @return Bitmap with glyph pixels */
        const TPtrShared<Image> &getBitmap() const { return mBitmap; }

        /** @return Default import options */
        static const TPtrShared<FontImportOptions> &getDefaultImportOptions();

        /** @copydoc Resource::showDebugInfo() */
        void showDebugInfo() override;

        /** Class info */
        void registerInfo();

        /** @return Loaded Gpu font for rendering (debug) */
        static TPtrShared<Render::GpuFont> loadGpuFont(uint32 size, const CString& fontPath);

    private:

        /** Map codepoint to the glyph index */
        GlyphsIdx mGlyphsIdx;

        /** Actual glyphs data */
        Glyphs mGlyphs;

        /** Font human readable name (may be not unique) */
        CString mFontName;

        /** Font size in px */
        Size2i mFontSize;

        /** Bitmap with glyph pixels */
        TPtrShared<Image> mBitmap;
    };

}

#endif //BERSERK_FONT_H
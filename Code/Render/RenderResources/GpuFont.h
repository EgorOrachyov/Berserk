/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GPUFONT_H
#define BERSERK_GPUFONT_H

#include <RenderResources/GpuFontGlyph.h>
#include <RenderResources/Texture2D.h>
#include <RenderResources/RenderResource.h>
#include <String/CString.h>
#include <String/WString.h>

namespace Berserk {
    namespace Render {

        /** Render module font resource with RHI 2D texture bitmap */
        class GpuFont : public RenderResource {
        public:

            /** Typedefs for common containers */
            using Glyphs = TArray<GpuFontGlyph>;
            using GlyphsIdx = TMap<wchar,uint32,THashRaw<wchar>>;

            /** Printed glyph when failed to find required glyphs */
            static const wchar NULL_GLYPH = 0;

            /** Create from specified properties and data */
            GpuFont(Size2i size, CString name, Glyphs glyphs, GlyphsIdx glyphsIdx, TPtrShared<Image> bitmap);

            ~GpuFont() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;

            /** @return Text size for specified string in this font*/
            Size2i getTextSize(const wchar* text) const;

            /** @return Text size for specified string in this font*/
            Size2i getTextSize(const char* text) const;

            /** @return Text size for specified string in this font*/
            Size2i getTextSize(const WString &text) const;

            /** @return Text size for specified string in this font*/
            Size2i getTextSize(const CString &text) const;

            /** @return Attempts to get glyph info (true if has glyph for the code point)*/
            bool getGlyph(wchar codepoint, GpuFontGlyph& glyph) const;

            /** @return Glyphs indices by codepoint */
            const GlyphsIdx& getGlyphsIdx() const { return mGlyphsIdx; }

            /** @return Array of glyphs */
            const Glyphs& getGlyphs() const { return mGlyphs; }

            /** @return Font name */
            const CString& getFontName() const { return mFontName; }

            /** @return Font size in px */
            const Size2i& getFontSize() const { return mFontSize; }

            /** @return Bitmap glyphs atlas texture */
            const TPtrShared<Texture2D> getBitmapTexture() const { return mBitmap; }

        private:

            GlyphsIdx mGlyphsIdx;
            Glyphs mGlyphs;
            CString mFontName;
            Size2i mFontSize;
            TPtrShared<Texture2D> mBitmap;
        };

    }
}



#endif //BERSERK_GPUFONT_H

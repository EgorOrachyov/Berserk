/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/GpuFont.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        GpuFont::GpuFont(const Berserk::Font &font) {
            BERSERK_COND_ERROR_RET(font.isLoaded(), "Provided font resource is not loaded");

            mGlyphsIdx = font.getGlyphsIdx();
            mGlyphs = font.getGlyphs();
            mFontName = font.getFontName();
            mFontSize = font.getFontSize();

            Image& image = *font.getBitmap();
            mBitmap = TPtrShared<Texture2D>::make(image.getName(), image, true);

            RHISamplerDesc samplerDesc;
            samplerDesc.min = ESamplerFilter::Linear;
            samplerDesc.mag = ESamplerFilter::Linear;
            samplerDesc.mipmapMode = ESamplerFilter::Linear;
            samplerDesc.useMips = true;
            samplerDesc.u = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.v = ESamplerRepeatMode::ClampToBorder;
            samplerDesc.color = ESamplerBorderColor::Black;

            auto sampler = RHIDevice::getSingleton().createSampler(samplerDesc);
            mBitmap->setSampler(std::move(sampler));
        }

        bool GpuFont::isInitialized() const {
            return mBitmap.isNotNull();
        }

        bool GpuFont::isInitializedRHI() const {
            return mBitmap.isNotNull();
        }

        CString GpuFont::getFriendlyName() const {
            return mFontName;
        }

        Size2i GpuFont::getTextSize(const wchar *text) const {
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

        Size2i GpuFont::getTextSize(const char *text) const {
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

        Size2i GpuFont::getTextSize(const WString &text) const {
            return GpuFont::getTextSize(text.data());
        }

        Size2i GpuFont::getTextSize(const CString &text) const {
            return GpuFont::getTextSize(text.data());
        }


        bool GpuFont::getGlyph(wchar codepoint, FontGlyph &glyph) const {
            auto found = mGlyphsIdx.getPtr(codepoint);

            if (found.isNotNull()) {
                glyph = mGlyphs[*found];
                return true;
            }

            return false;
        }

    }
}
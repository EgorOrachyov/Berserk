//
// Created by Egor Orachyov on 2019-07-27.
//

#include "Rendering/Font.h"

namespace Berserk
{

    Font::Font(Berserk::String &name, Berserk::uint32 width, Berserk::uint32 height,
               Berserk::TArray<Berserk::CharData> &data, TSharedPtr<ImageData> bitmap)
               : mWidth(width),
                 mHeight(height),
                 mFontName(name),
                 mBitmap(std::move(bitmap)),
                 mCharsData(std::move(data))
    {

    }

    void Font::getStringSize(const char *source, Berserk::uint32 &outWidth, Berserk::uint32 &outHeight) const
    {
        uint32 width = 0;
        uint32 height = 0;

        while (*source != '\0')
        {
            char c = *source;
            const CharData& glyph = mCharsData.get(c);

            width += glyph.advanceX;
            height = Math::max((uint32) glyph.advanceY, height);

            source += 1;
        }

        outWidth = width;
        outHeight = height;
    }

    uint32 Font::getStringWidth(const char *source) const
    {
        uint32 width = 0;
        uint32 height = 0;

        getStringSize(source, width, height);

        return width;
    }

    uint32 Font::getStringHeight(const char *source) const
    {
        uint32 width = 0;
        uint32 height = 0;

        getStringSize(source, width, height);

        return height;
    }

    uint32 Font::getMemoryUsage() const
    {
        return sizeof(Font) + mCharsData.getMemoryUsage();
    }

    const String &Font::getName() const
    {
        return mFontName;
    }

} // namespace Berserk
//
// Created by Egor Orachyov on 2019-08-10.
//

#ifndef BERSERK_FONTIMPORTDATA_H
#define BERSERK_FONTIMPORTDATA_H

#include <Math/MathInclude.h>
#include <Rendering/PixelData.h>
#include <Pointer/TSharedPtr.h>
#include <Misc/Allocatable.h>

namespace Berserk
{

    /** Loaded glyph data for single char */
    struct ENGINE_API CharData
    {
    public:

        /** Meaning of this character */
        char codePoint = 0;

        /** Character glyph width in pixels */
        uint32 width = 0;

        /** Character glyph height in pixels */
        uint32 height = 0;

        /** Offset from baseline to left of glyph in pixels */
        int32 bearingX = 0;

        /** Offset from baseline to top of glyph in pixels */
        int32 bearingY = 0;

        /** Offset to advance to next glyph in X axis in pixels */
        int32 advanceX = 0;

        /** Offset to advance to next glyph in Y axis in pixels */
        int32 advanceY = 0;

        /** Left/Bottom, Top/Right glyph corners positions in font texture */
        Vec4f texturePos = Vec4f();

    };

    /** Data imported by font importer to load single font */
    class ENGINE_API FontImportData : public Allocatable
    {
    public:

        /**
         * Called by font importer
         * Creates font import data from char data array and defined size
         * @param name Font name (for view/debug)
         * @param width Max width of the font glyph in pixels
         * @param height Max height of the font glyph in pixels
         * @param data Array of glyph data
         * @param bitmap Texture bitmap with glyph rendering representation
         */
        FontImportData(String& name, uint32 width, uint32 height, TArray<CharData> &data, PixelData& bitmap)
            : mWidth(width),
              mHeight(height),
              mFontName(std::move(name)),
              mCharData(std::move(data)),
              mBitmap(std::move(bitmap))
        {

        }

        uint32 getWidth() const { return mWidth; }

        uint32 getHeight() const { return mHeight; }

        const String& getFontName() const { return mFontName; }

        TArray<CharData> &getCharData() { return mCharData; }

        PixelData &getBitmap() { return mBitmap; }

    protected:

        uint32 mWidth;
        uint32 mHeight;
        String mFontName;
        TArray<CharData> mCharData;
        PixelData mBitmap;

    };

} // namespace Berserk

#endif //BERSERK_FONTIMPORTDATA_H
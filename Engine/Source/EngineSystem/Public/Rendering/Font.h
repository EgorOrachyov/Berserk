//
// Created by Egor Orachyov on 2019-07-27.
//

#ifndef BERSERK_FONT_H
#define BERSERK_FONT_H

#include <Resource/IResource.h>
#include <Math/MathInclude.h>
#include <Rendering/ImageImportData.h>
#include <Resource/TSharedPtr.h>
#include <Object/Allocatable.h>

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

    /** Ascii font resource, contains chars data and rendering texture bitmap */
    class ENGINE_API Font : public IResource, public Allocatable
    {
    public:

        /**
         * Called by font importer
         * Creates new font object from char data array and defined size
         * @param name Font name (for view/debug)
         * @param width Max width of the font glyph in pixels
         * @param height Max height of the font glyph in pixels
         * @param data Array of glyph data
         * @param bitmap Texture bitmap with glyph rendering representation
         */
        Font(String& name, uint32 width, uint32 height, TArray<CharData> &data, TSharedPtr<ImageImportData> bitmap);

        ~Font() override = default;

        /**
         * Compute pixel size of ascii string in this font
         * @param source C-style string to compute size
         * @param[out] outWidth Pixel width of the string
         * @param[out] outHeight Pixel height of the string
         */
        void getStringSize(const char* source, uint32& outWidth, uint32& outHeight) const;

        /**
         * Compute pixel size of ascii string in this font
         * @param source C-style string to compute size
         * @return String width in pixels
         */
        uint32 getStringWidth(const char* source) const;

        /**
         * Compute pixel size of ascii string in this font
         * @param source C-style string to compute size
         * @return String height in pixels
         */
        uint32 getStringHeight(const char* source) const;

        /** @return Pixel width of this font */
        uint32 getWidth() const { return mWidth; }

        /** @return Pixel height of this font */
        uint32 getHeight() const { return mHeight; }

        /** @return This font bitmap [packed glyphs texture] */
        TSharedPtr<ImageImportData> getBitmap() const { return mBitmap; }

        /** @return Char data array */
        const TArray<CharData> &getCharData() const { return mCharsData; }

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() const override
        { return sizeof(Font) +
                 mCharsData.getMemoryUsage() +
                 mBitmap->getMemoryUsage();
        }

        /** @copydoc IResource::getName() */
        const String &getName() const override { return mFontName; }

    public:

        /** Num of char faces to load for font ascii encoding */
        static const uint32 ENCODING_ASCII = 128;

        /** Bitmap offset between faces to avoid blending of neighbour glyphs */
        static const uint32 BITMAP_OFFSET = 2;

    private:

        uint32 mWidth;
        uint32 mHeight;
        String mFontName;
        TSharedPtr<ImageImportData> mBitmap;
        TArray<CharData> mCharsData;

    };



} // namespace Berserk

#endif //BERSERK_FONT_H
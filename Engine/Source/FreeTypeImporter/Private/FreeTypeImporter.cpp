//
// Created by Egor Orachyov on 2019-07-30.
//

#include <FreeTypeImporter.h>
#include <Logging/DebugLogMacros.h>

namespace Berserk
{

    /** Called as empty result */
    static TSharedPtr<FontImportData> nullresult()
    {
        return TSharedPtr<FontImportData>();
    }

    FreeTypeImporter::FreeTypeImporter(Berserk::IAllocator &allocator)
        : mAllocator(allocator)
    {
        auto error = FT_Init_FreeType(&mLibrary);

        if (error)
        {
            DEBUG_LOG_ERROR("FreeTypeImporter: cannot initialize library");
            return;
        }
    }

    FreeTypeImporter::~FreeTypeImporter()
    {
        FT_Done_FreeType(mLibrary);
    }

    TSharedPtr<FontImportData> FreeTypeImporter::load(const char *filename, const char *fontname, uint32 fontSize)
    {
        const uint32 ENCODING = Font::ENCODING_ASCII;
        const uint32 OFFSET = Font::BITMAP_OFFSET;
        const EDataType  dataType = DT_UnsignedByte;
        const EPixelFormat pixelFormat = PF_R;
        const EStorageFormat storageFormat = SF_R8;

        FT_Face face;

        if (FT_New_Face(mLibrary, filename, 0, &face))
        {
            DEBUG_LOG_ERROR("FreeTypeImporter: cannot load font from file [name: '%s']", filename);
            return nullresult();
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);

        uint32 fontWidth = 0;
        uint32 fontHeight = fontSize;

        uint32 bitmapWidth = 0;
        uint32 bitmapHeight = 0;

        TArray<uint8> bitmaps(fontSize * fontSize * ENCODING, mAllocator);
        TArray<CharData> charData(Font::ENCODING_ASCII, mAllocator);

        for (uint32 i = 0; i < ENCODING; i++)
        {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                DEBUG_LOG_WARNING("FreeTypeImporter: cannot load char [name: '%s']", filename);

                /** Add empty glyph to be sure, that char look-up works fie in array */
                CharData glyph;
                charData.add(glyph);

                continue;
            }

            CharData glyph;

            glyph.codePoint = (char) i;

            glyph.width     = face->glyph->bitmap.width;
            glyph.height    = face->glyph->bitmap.rows;

            glyph.bearingX  = face->glyph->bitmap_left;
            glyph.bearingY  = face->glyph->bitmap_top;

            glyph.advanceX  = (uint32) (face->glyph->advance.x >> 6);
            glyph.advanceY  = (uint32) (face->glyph->advance.y >> 6);

            charData.add(glyph);
            bitmaps.append(face->glyph->bitmap.buffer, glyph.width * glyph.height);

            bitmapWidth = Math::max(bitmapWidth, glyph.width);
            bitmapHeight += glyph.height;

            fontWidth = Math::max(fontWidth, glyph.width);
        }

        uint32 atlasWidth = bitmapWidth;
        uint32 atlasHeight = OFFSET * (ENCODING - 1) + bitmapHeight;
        uint32 atlasSize = atlasWidth * atlasHeight;

        String imageName = filename;
        ImageImportData imageData(
                atlasWidth,
                atlasHeight,
                dataType,
                pixelFormat,
                storageFormat,
                atlasSize,
                mAllocator);

        uint32 x = 0;
        uint32 y = 0;

        int32 write = 0;
        int32 read = 0;

        uint8* source = bitmaps.getRawBuffer();
        uint8* target = imageData.getBuffer();

        memset(target, 0x0, sizeof(uint8) * atlasSize);

        for (int32 i = 0; i < charData.getSize(); i++)
        {
            CharData& data = charData.get(i);

            read += data.width * data.height;
            uint32 readOffset = read - data.width;

            for (int32 j = data.height; j > 0; j--)
            {
                memcpy(&target[write], &source[readOffset], sizeof(uint8) * data.width);

                write += atlasWidth;
                readOffset -= data.width;
            }

            float32 u = x;
            float32 v = (float32) y / (float32) atlasHeight;
            float32 w = (float32) data.width / (float32) atlasWidth;
            float32 h = (float32) data.height / (float32) atlasHeight;

            data.texturePos = Vec4f(u, v, u + w, v + h);

            y += OFFSET + data.height;
            write += atlasWidth * OFFSET;

///////////////////////////////////////////////////////////////////////////////////////
/// Debug print of the loaded glyphs bitmaps in
///
///            uint32 poss = read - data.width * data.height;
///            for (uint32 h = 0; h < data.height; h++) {
///                for (uint32 w = 0; w < data.width; w++) {
///                    if (source[poss] != 0) OutputDevice::printf("%u", source[poss] % 10);
///                    else OutputDevice::printf(" ");
///                    poss += 1;
///                }
///                printf("\n");
///            }
///            printf("\n");
///
///////////////////////////////////////////////////////////////////////////////////////

        }

        String name(fontname);

        auto font = mAllocator.engnie_new<FontImportData>(
                name,
                fontWidth,
                fontHeight,
                charData,
                imageData);

        return TSharedPtr<FontImportData>(font, &mAllocator);
    }

} // namespace Berserk
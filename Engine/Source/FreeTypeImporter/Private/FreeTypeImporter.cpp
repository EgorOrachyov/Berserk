//
// Created by Egor Orachyov on 17.04.2019.
//

#include "FreeTypeImporter.h"

namespace Berserk::Importers
{

    FreeTypeImporter::FreeTypeImporter() : mBuffer(INITIAL_BUFFER_SIZE)
    {
        auto error = FT_Init_FreeType(&mLibrary);

        if (error)
        {
            FAIL(false, "Cannot initialize FreeType library");
        }

        PUSH("FreeTypeImporter: initialize");
    }

    FreeTypeImporter::~FreeTypeImporter()
    {
        FT_Done_FreeType(mLibrary);
        PUSH("FreeTypeImporter: de-initialize");
    }

    bool FreeTypeImporter::import(const char *name, uint32 pixelSize, Resources::IFont *font)
    {
        const uint32 ASCII7 = 128;

        FT_Face face;

        if (FT_New_Face(mLibrary, name, 0, &face))
        {
            WARNING("Cannot load font from file [name: '%s']", name);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelSize);

        uint32 totalWidth = 0;
        uint32 totalHeight = 0;
        ArrayList<Resources::Character>& data = *font->getCharacters();

        for (uint32 i = 0; i < ASCII7; i++)
        {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                continue;
            }

            Resources::Character character;
            character.codepoint = (char) i;
            character.width = face->glyph->bitmap.width;
            character.height = face->glyph->bitmap.rows;
            character.bearingX = face->glyph->bitmap_left;
            character.bearingY = face->glyph->bitmap_top;
            character.advanceX = (uint32) (face->glyph->advance.x >> 6);
            character.advanceY = (uint32) (face->glyph->advance.y >> 6);

            data += character;
            mBuffer.add(character.width * character.height, (char*)face->glyph->bitmap.buffer);

            totalHeight += character.height;
            totalWidth = Math::max(character.width, totalWidth);

#if DEBUG_PRINT_LOADED_FONT

            printf("Load char: %c (w %u,h %u) (bx %i, by %i) (ax %u, ay %u) \n",
                   character.codepoint,
                   character.width,
                   character.height,
                   character.bearingX,
                   character.bearingY,
                   character.advanceX,
                   character.advanceY);

            for (uint32 i = 0; i < character.height; i++)
            {
                for (uint32 j = 0; j < character.width ; j++)
                {
                    if (face->glyph->bitmap.buffer[i * character.width + j] != 0)
                    {
                        printf("%u", face->glyph->bitmap.buffer[i * character.width + j] % 10);
                    }
                    else
                    {
                        printf(" ");
                    }
                }

                printf("\n");
            }

#endif

        }

        // printf("Total width: %u height: %u\n", totalWidth, totalHeight);

        ArrayList<char> texture(totalWidth * totalHeight);
        char* buffer = texture.get();
        char* source = mBuffer.get();

        memset(buffer, 0x0, sizeof(char) * totalWidth * totalHeight);

        uint32 read = 0;
        uint32 write = 0;

        for (uint32 c = 0; c < data.getSize(); c++)
        {
            read += data[c].width * data[c].height;
            uint32 offset = 0;

            for (uint32 i = data[c].height; i > 0; i--)
            {
                offset += data[c].width;

                memcpy(&buffer[write], &source[read - offset], sizeof(char) * data[c].width);
                write += totalWidth;
            }
        }

        // printf("Total read: %u write: %u\n", read, write);

#if DEBUG_PRINT_LOADED_FONT

        for (uint32 i = 0; i < totalHeight; i++)
        {
            for (uint32 j = 0; j < totalWidth; j++)
            {
                if (buffer[i * totalWidth + j] != 0)
                {
                    printf("%u", ((unsigned char) buffer[i * totalWidth + j]) % 10);
                }
                else
                {
                    printf(" ");
                }
            }

            printf("\n");
        }

#endif

        Resources::ITexture* glyphs = font->getTexture();
        glyphs->create(totalWidth,
                       totalHeight,
                       Resources::IRenderDriver::StorageFormat::R8,
                       Resources::IRenderDriver::PixelFormat::R,
                       Resources::IRenderDriver::DataType::UNSIGNED_BYTE,
                       buffer,
                       false);

#if PROFILE_FONT_IMPORTER
        PUSH("FreeTypeImporter: load font [font: '%s'][pixel size: %u]", name, pixelSize);
#endif

        mBuffer.reset();
        FT_Done_Face(face);
    }

    uint32 FreeTypeImporter::getMemoryUsage()
    {
        return sizeof(FreeTypeImporter) + mBuffer.getMemoryUsage();
    }

} // namespace Berserk::Importers
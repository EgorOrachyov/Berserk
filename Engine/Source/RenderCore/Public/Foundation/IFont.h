//
// Created by Egor Orachyov on 17.04.2019.
//

#ifndef BERSERK_IFONT_H
#define BERSERK_IFONT_H

#include <Math/MathInclude.h>
#include <Resource/IResource.h>
#include <Platform/ITexture.h>
#include <Containers/ArrayList.h>

namespace Berserk::Resources
{

    /** One character info in font */
    struct ENGINE_API Character
    {
        char codepoint = 0;         //! Meaning of this character

        uint32 width = 0;           //! Character glyph width in pixels
        uint32 height = 0;          //! Character glyph height in pixels

        int32 bearingX = 0;         //! Offset from baseline to left of glyph in pixels
        int32 bearingY = 0;         //! Offset from baseline to top of glyph in pixels

        int32 advanceX = 0;        //! Offset to advance to next glyph in X axis in pixels
        int32 advanceY = 0;        //! Offset to advance to next glyph in Y axis in pixels

        uint32 texturePosX = 0;     //! Left/Bottom glyph corner x position in font texture
        uint32 texturePosY = 0;     //! Left/Bottom glyph corner y position in font texture
    };

    /**
     * Font resource stores all the characters and its data,
     * needed for the proper text rendering
     */
    class ENGINE_API IFont : public IResource
    {
    public:

        /** @return Font size in pixels of bitmap */
        virtual uint32 getPixelWidth() = 0;

        /** @return Font size in pixels of bitmap */
        virtual uint32 getPixelHeight() = 0;

        /** @return Single texture map, which stores all the glyphs bitmaps */
        virtual ITexture* getTexture() = 0;

        /** @return Array with all [default ASCII-7] characters info */
        virtual ArrayList<Character>* getCharacters() = 0;

    };

} // namespace Berserk::Resources

#endif //BERSERK_IFONT_H

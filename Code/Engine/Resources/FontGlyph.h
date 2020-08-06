/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FONTGLYPH_H
#define BERSERK_FONTGLYPH_H

#include <Math/Vec4f.h>
#include <Math/Point2i.h>

namespace Berserk {

    /** Represent single printable character */
    struct FontGlyph {

        /** Code point */
        wchar codepoint;

        /** Character glyph width and height in pixels */
        Size2i size;

        /** Offset from baseline to left of glyph and to top of glyph in pixels */
        Size2i bearing;

        /** Offset to advance to next glyph in X and Y axis in pixels */
        Size2i advance;

        /** Left/Bottom, Top/Right glyph corners positions in font texture */
        Vec4f uvs;

        /** For font serialization */
        friend Archive& operator<<(Archive& archive, const FontGlyph& glyph) {
            archive << glyph.codepoint;
            archive << glyph.size;
            archive << glyph.bearing;
            archive << glyph.advance;
            archive << glyph.uvs;

            return archive;
        }

        /** For font deserialization */
        friend Archive& operator>>(Archive& archive, FontGlyph& glyph) {
            archive >> glyph.codepoint;
            archive >> glyph.size;
            archive >> glyph.bearing;
            archive >> glyph.advance;
            archive >> glyph.uvs;

            return archive;
        }

    };

}

#endif //BERSERK_FONTGLYPH_H
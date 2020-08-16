/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GPUFONTGLYPH_H
#define BERSERK_GPUFONTGLYPH_H

#include <Math/Vec4f.h>
#include <Math/Point2i.h>
#include <IO/Archive.h>

namespace Berserk {
    namespace Render {

        /** Represent single printable character */
        struct GpuFontGlyph {

            /** Code point */
            wchar codepoint = 0;

            /** Character glyph width and height in pixels */
            Size2i size;

            /** Offset from baseline to left of glyph and to top of glyph in pixels */
            Size2i bearing;

            /** Offset to advance to next glyph in X and Y axis in pixels */
            Size2i advance;

            /** Left/Top, Right/Bottom glyph corners positions in font texture */
            Vec4f uvs;

            /** For font serialization */
            friend Archive& operator<<(Archive& archive, const GpuFontGlyph& glyph) {
                archive << glyph.codepoint;
                archive << glyph.size;
                archive << glyph.bearing;
                archive << glyph.advance;
                archive << glyph.uvs;

                return archive;
            }

            /** For font deserialization */
            friend Archive& operator>>(Archive& archive, GpuFontGlyph& glyph) {
                archive >> glyph.codepoint;
                archive >> glyph.size;
                archive >> glyph.bearing;
                archive >> glyph.advance;
                archive >> glyph.uvs;

                return archive;
            }

        };
    }

}

#endif //BERSERK_GPUFONTGLYPH_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_COLOR4_H
#define BERSERK_COLOR4_H

#include <Math/Vec4f.h>
#include <Math/TVecN.h>

namespace Berserk {

    /**
     * @brief Unsigned byte based color
     *
     * Platform byte order independent color class.
     */
    class Color4u : public TVecN<uint8, 4> {
    public:
        using  TVecN<uint8, 4>::TVecN;
        using  TVecN<uint8, 4>::operator=;
        using  TVecN<uint8, 4>::values;

        static const uint32 COMP_0 = 0;
        static const uint32 COMP_1 = 1;
        static const uint32 COMP_2 = 2;
        static const uint32 COMP_3 = 3;

        static const uint32 COMP_R = 0;
        static const uint32 COMP_G = 1;
        static const uint32 COMP_B = 2;
        static const uint32 COMP_A = 3;

        /**
         * @note Inverse operation is fromR8G8B8A8()
         * @return Color encoded as rgba 8 where byte[0] - R, ... , byte[3] - A (independent representation)
         */
        uint32 toR8G8B8A8() const;

        /**
         * @note Inverse operation is toR8G8B8A8()
         * @return Color4u decoded as rgba 8 where byte[0] - R, ... , byte[3] - A (independent representation)
         */
        static Color4u fromR8G8B8A8(uint32 color);

        uint8 getR() const { return values[COMP_R]; }
        uint8 getG() const { return values[COMP_G]; }
        uint8 getB() const { return values[COMP_B]; }
        uint8 getA() const { return values[COMP_A]; }
    };

    /**
     * @brief Float based color
     *
     * Color value, stores data as RGBA float where each value normally from [0..1]
     * @note Alpha component (A) always implicitly set to 1.0f in cases, when A is not provided.
     */
    class Color4f : public Vec4f {
    public:
        using Vec4f::Vec4f;
        using Vec4f::operator=;
        using Vec4f::values;

        static const uint32 COMP_0 = 0;
        static const uint32 COMP_1 = 1;
        static const uint32 COMP_2 = 2;
        static const uint32 COMP_3 = 3;

        static const uint32 COMP_R = 0;
        static const uint32 COMP_G = 1;
        static const uint32 COMP_B = 2;
        static const uint32 COMP_A = 3;

        Color4f(float mono);
        Color4f(float r, float g, float b);
        Color4f(uint32 r, uint32 g, uint32 b);
        Color4f(const TVecN<float,4>& v) : Vec4f(v) {}

        uint32 toR8G8B8A8() const;
        uint32 toA8R8G8B8() const;
        uint32 toA8B8G8R8() const;
        uint8  toR8() const;

        Color4u toColorR8G8B8A8() const;

        float gray() const;
        float lightness() const;
        float luminance() const;
        Color4f invert() const;
        Color4f toSRGB(float factor = 1.0f/2.2f) const;
        Color4f toLinear(float factor = 2.2f) const;

        static Color4f fromR8G8B8A8(uint32 color);
        static Color4f fromA8R8G8B8(uint32 color);
        static Color4f fromA8B8G8R8(uint32 color);
        static Color4f fromR8(uint8 color);
        static Color4f fromHex(uint32 rgba);

        static Color4f fromR8G8B8A8(const Color4u &color);

        float getR() const { return values[COMP_R]; }
        float getG() const { return values[COMP_G]; }
        float getB() const { return values[COMP_B]; }
        float getA() const { return values[COMP_A]; }

        float& R() { return values[COMP_R]; }
        float& G() { return values[COMP_G]; }
        float& B() { return values[COMP_B]; }
        float& A() { return values[COMP_A]; }
    };

}

#endif //BERSERK_COLOR4_H
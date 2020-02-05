/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_COLOR4F_H
#define BERSERK_COLOR4F_H

#include <Math/Vec4f.h>

namespace Berserk {

    /** Color value, stores data as RGBA float32 where each value normally from [0..1] */
    class Color4f : public Vec4f {
    public:
        using Vec4f::Vec4f;
        using Vec4f::operator=;
        using Vec4f::values;

        static const uint32 COMP_R = 0;
        static const uint32 COMP_G = 1;
        static const uint32 COMP_B = 2;
        static const uint32 COMP_A = 3;

        Color4f(float32 mono);
        Color4f(float32 r, float32 g, float32 b);
        Color4f(uint32 r, uint32 g, uint32 b);

        uint32 toR8G8B8A8() const;
        uint32 toA8R8G8B8() const;
        uint32 toA8B8G8R8() const;

        float32 gray() const;
        float32 lightness() const;
        float32 luminance() const;
        Color4f invert() const;
        Color4f toSRGB(float32 factor = 1.0f/2.2f) const;
        Color4f toLinear(float32 factor = 2.2f) const;

        static Color4f fromR8G8B8A8(uint32 color);
        static Color4f fromA8R8G8B8(uint32 color);
        static Color4f fromA8B8G8R8(uint32 color);
    };

}

#endif //BERSERK_COLOR4F_H
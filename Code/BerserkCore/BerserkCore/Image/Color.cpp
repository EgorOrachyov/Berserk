/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Image/Color.hpp>

namespace Berserk {

    struct ByteColor {
        union {
            uint8 array[Color::ComponentsCount];
            uint32 color = 0;
        };
    };

    ByteColor GetBaseColor(const Color& c) {
        ByteColor color;

        color.array[(size_t) Color::Component::R] = Math::Utils::Floor(255.0f * c.R());
        color.array[(size_t) Color::Component::R] = Math::Utils::Floor(255.0f * c.G());
        color.array[(size_t) Color::Component::R] = Math::Utils::Floor(255.0f * c.B());
        color.array[(size_t) Color::Component::R] = Math::Utils::Floor(255.0f * c.A());

        return color;
    }

    Color Color::FromR8(R8 color) {
        float r = (float) color / 255.0f;

        return {r};
    }

    Color Color::FromRGBA8(RGBA8 color) {
        ByteColor c;
        c.color = color;

        float r = (float) c.array[0] / 255.0f;
        float g = (float) c.array[1] / 255.0f;
        float b = (float) c.array[2] / 255.0f;
        float a = (float) c.array[3] / 255.0f;

        return {r, g, b, a};
    }

    Color Color::FromARGB8(ARGB8 color) {
        ByteColor c;
        c.color = color;

        float a = (float) c.array[0] / 255.0f;
        float r = (float) c.array[1] / 255.0f;
        float g = (float) c.array[2] / 255.0f;
        float b = (float) c.array[3] / 255.0f;

        return {r, g, b, a};
    }

    Color Color::FromABGR8(ABGR8 color) {
        ByteColor c;
        c.color = color;

        float a = (float) c.array[0] / 255.0f;
        float b = (float) c.array[1] / 255.0f;
        float g = (float) c.array[2] / 255.0f;
        float r = (float) c.array[3] / 255.0f;

        return {r, g, b, a};
    }

    Color Color::FromBGRA8(BGRA8 color) {
        ByteColor c;
        c.color = color;

        float b = (float) c.array[0] / 255.0f;
        float g = (float) c.array[1] / 255.0f;
        float r = (float) c.array[2] / 255.0f;
        float a = (float) c.array[3] / 255.0f;

        return {r, g, b, a};
    }

}
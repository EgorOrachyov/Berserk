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
        color.array[(size_t) Color::Component::G] = Math::Utils::Floor(255.0f * c.G());
        color.array[(size_t) Color::Component::B] = Math::Utils::Floor(255.0f * c.B());
        color.array[(size_t) Color::Component::A] = Math::Utils::Floor(255.0f * c.A());

        return color;
    }

    Color::R8 Color::ToR8() const {
        uint8 r = Math::Utils::Floor(R() * 255.0f);

        return r;
    }

    Color::RGBA8 Color::ToRGBA8() const {
        auto base = GetBaseColor(*this);

        return base.color;
    }

    Color::ARGB8 Color::ToARGB8() const {
        auto base = GetBaseColor(*this);
        auto result = ByteColor();

        result.array[0] = base.array[(size_t) Color::Component::A];
        result.array[1] = base.array[(size_t) Color::Component::R];
        result.array[2] = base.array[(size_t) Color::Component::G];
        result.array[3] = base.array[(size_t) Color::Component::B];

        return result.color;
    }

    Color::ABGR8 Color::ToABGR8() const {
        auto base = GetBaseColor(*this);
        auto result = ByteColor();

        result.array[0] = base.array[(size_t) Color::Component::A];
        result.array[1] = base.array[(size_t) Color::Component::B];
        result.array[2] = base.array[(size_t) Color::Component::G];
        result.array[3] = base.array[(size_t) Color::Component::R];

        return result.color;
    }

    Color::BGRA8 Color::ToBGRA8() const {
        auto base = GetBaseColor(*this);
        auto result = ByteColor();

        result.array[0] = base.array[(size_t) Color::Component::B];
        result.array[1] = base.array[(size_t) Color::Component::G];
        result.array[2] = base.array[(size_t) Color::Component::R];
        result.array[3] = base.array[(size_t) Color::Component::A];

        return result.color;
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
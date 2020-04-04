/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Math/Color4.h>

namespace Berserk {

    uint32 Color4u::toR8G8B8A8() const {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.bytes[COMP_0] = values[COMP_R];
        t.bytes[COMP_1] = values[COMP_G];
        t.bytes[COMP_2] = values[COMP_B];
        t.bytes[COMP_3] = values[COMP_A];

        return t.value;
    }

    Color4u Color4u::fromR8G8B8A8(uint32 color) {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.value = color;

        Color4u r;

        r.values[COMP_R] = t.bytes[COMP_0];
        r.values[COMP_G] = t.bytes[COMP_1];
        r.values[COMP_B] = t.bytes[COMP_2];
        r.values[COMP_A] = t.bytes[COMP_3];

        return r;
    }

    Color4f::Color4f(float32 mono) {
        values[COMP_R] = mono;
        values[COMP_G] = mono;
        values[COMP_B] = mono;
        values[COMP_A] = 1.0f;
    }

    Color4f::Color4f(float32 r, float32 g, float32 b) {
        values[COMP_R] = r;
        values[COMP_G] = g;
        values[COMP_B] = b;
        values[COMP_A] = 1.0f;
    }

    Color4f::Color4f(uint32 r, uint32 g, uint32 b) {
        values[COMP_R] = (float32)r / 255.0f;
        values[COMP_G] = (float32)g / 255.0f;
        values[COMP_B] = (float32)b / 255.0f;
        values[COMP_A] = 1.0f;
    }

    uint32 Color4f::toR8G8B8A8() const {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.bytes[COMP_0] = (uint8)Math::round(values[COMP_R] * 255.0f);
        t.bytes[COMP_1] = (uint8)Math::round(values[COMP_G] * 255.0f);
        t.bytes[COMP_2] = (uint8)Math::round(values[COMP_B] * 255.0f);
        t.bytes[COMP_3] = (uint8)Math::round(values[COMP_A] * 255.0f);

        return t.value;
    }

    uint32 Color4f::toA8R8G8B8() const {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.bytes[COMP_0] = (uint8)Math::round(values[COMP_A] * 255.0f);
        t.bytes[COMP_1] = (uint8)Math::round(values[COMP_R] * 255.0f);
        t.bytes[COMP_2] = (uint8)Math::round(values[COMP_G] * 255.0f);
        t.bytes[COMP_3] = (uint8)Math::round(values[COMP_B] * 255.0f);

        return t.value;
    }

    uint32 Color4f::toA8B8G8R8() const {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.bytes[COMP_0] = (uint8)Math::round(values[COMP_A] * 255.0f);
        t.bytes[COMP_1] = (uint8)Math::round(values[COMP_B] * 255.0f);
        t.bytes[COMP_2] = (uint8)Math::round(values[COMP_G] * 255.0f);
        t.bytes[COMP_3] = (uint8)Math::round(values[COMP_R] * 255.0f);

        return t.value;
    }

    uint8 Color4f::toR8() const {
        return (uint8) Math::round(values[COMP_R] * 255.0f);
    }

    Color4u Color4f::toColorR8G8B8A8() const {
        Color4u color;

        color.values[COMP_R] = (uint8)Math::round(values[COMP_R] * 255.0f);
        color.values[COMP_G] = (uint8)Math::round(values[COMP_G] * 255.0f);
        color.values[COMP_B] = (uint8)Math::round(values[COMP_B] * 255.0f);
        color.values[COMP_A] = (uint8)Math::round(values[COMP_A] * 255.0f);

        return color;
    }


    float32 Color4f::gray() const {
        return dot(*this, Color4f(0.3f, 0.4f, 0.3f, 0.0f));
    }

    float32 Color4f::lightness() const {
        float32 max = Math::max(values[COMP_R], Math::max(values[COMP_G], values[COMP_B]));
        float32 min = Math::min(values[COMP_R], Math::min(values[COMP_G], values[COMP_B]));

        return (max + min) * 0.5f;
    }

    float32 Color4f::luminance() const {
        return dot(*this, Color4f(0.21f, 0.72f, 0.07f, 0.0f));
    }

    Color4f Color4f::invert() const {
        Color4f r;

        r.values[COMP_R] = 1.0f - values[COMP_R];
        r.values[COMP_G] = 1.0f - values[COMP_G];
        r.values[COMP_B] = 1.0f - values[COMP_B];

        return r;
    }

    Color4f Color4f::toSRGB(float32 factor) const {
        Color4f r;

        for (uint32 i = 0; i < 4; i++) {
            r.values[i] = Math::pow(values[i], factor);
        }

        return r;
    }

    Color4f Color4f::toLinear(float32 factor) const {
        Color4f r;

        for (uint32 i = 0; i < 4; i++) {
            r.values[i] = Math::pow(values[i], factor);
        }

        return r;
    }
    
    Color4f Color4f::fromR8G8B8A8(uint32 color) {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.value = color;

        Color4f r;

        r.values[COMP_R] = (float32)(t.bytes[COMP_0]) / 255.0f;
        r.values[COMP_G] = (float32)(t.bytes[COMP_1]) / 255.0f;
        r.values[COMP_B] = (float32)(t.bytes[COMP_2]) / 255.0f;
        r.values[COMP_A] = (float32)(t.bytes[COMP_3]) / 255.0f;

        return r;
    }
    
    Color4f Color4f::fromA8R8G8B8(uint32 color) {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.value = color;

        Color4f r;

        r.values[COMP_A] = (float32)(t.bytes[COMP_0]) / 255.0f;
        r.values[COMP_R] = (float32)(t.bytes[COMP_1]) / 255.0f;
        r.values[COMP_G] = (float32)(t.bytes[COMP_2]) / 255.0f;
        r.values[COMP_B] = (float32)(t.bytes[COMP_3]) / 255.0f;

        return r;
    }
    
    Color4f Color4f::fromA8B8G8R8(uint32 color) {
        union {
            uint8 bytes[4];
            uint32 value;
        } t;

        t.value = color;

        Color4f r;

        r.values[COMP_A] = (float32)(t.bytes[COMP_0]) / 255.0f;
        r.values[COMP_B] = (float32)(t.bytes[COMP_1]) / 255.0f;
        r.values[COMP_G] = (float32)(t.bytes[COMP_2]) / 255.0f;
        r.values[COMP_R] = (float32)(t.bytes[COMP_3]) / 255.0f;

        return r;
    }

    Color4f Color4f::fromR8(uint8 color) {
        Color4f r;

        r.values[COMP_R] = (float32)(color) / 255.0f;
        r.values[COMP_A] = 1.0f;

        return r;
    }

    Color4f Color4f::fromR8G8B8A8(const Color4u &color) {
        Color4f r;

        r.values[COMP_R] = (float32)color.values[COMP_R] / 255.0f;
        r.values[COMP_G] = (float32)color.values[COMP_G] / 255.0f;
        r.values[COMP_B] = (float32)color.values[COMP_B] / 255.0f;
        r.values[COMP_A] = (float32)color.values[COMP_A] / 255.0f;

        return r;
    }

}
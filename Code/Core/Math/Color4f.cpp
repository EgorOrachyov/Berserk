/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Math/Color4f.h>

namespace Berserk {

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
        uint32 color = 0;

        color |= (uint8)Math::round(values[COMP_R] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_G] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_B] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_A] * 255.0f);

        return color;
    }

    uint32 Color4f::toA8R8G8B8() const {
        uint32 color = 0;

        color |= (uint8)Math::round(values[COMP_A] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_R] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_G] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_B] * 255.0f);

        return color;
    }

    uint32 Color4f::toA8B8G8R8() const {
        uint32 color = 0;

        color |= (uint8)Math::round(values[COMP_A] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_B] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_G] * 255.0f);
        color <<= 8u;
        color |= (uint8)Math::round(values[COMP_R] * 255.0f);

        return color;
    }

    uint8 Color4f::toR8() const {
        return (uint8) Math::round(values[COMP_R] * 255.0f);
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
        Color4f r;

        r.values[COMP_A] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_B] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_G] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_R] = (float32)(0xffu & color) / 255.0f;

        return r;
    }
    
    Color4f Color4f::fromA8R8G8B8(uint32 color) {
        Color4f r;

        r.values[COMP_B] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_G] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_R] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_A] = (float32)(0xffu & color) / 255.0f;

        return r;
    }
    
    Color4f Color4f::fromA8B8G8R8(uint32 color) {
        Color4f r;

        r.values[COMP_R] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_G] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_B] = (float32)(0xffu & color) / 255.0f;
        color >>= 8u;
        r.values[COMP_A] = (float32)(0xffu & color) / 255.0f;

        return r;
    }

    Color4f Color4f::fromR8(Berserk::uint8 color) {
        Color4f r;
        r.values[COMP_R] = (float32)(color) / 255.0f;

        return r;
    }

}
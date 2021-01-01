/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_COLOR_HPP
#define BERSERK_COLOR_HPP

#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Assert.hpp>

namespace Berserk {

    /**
     * @brief Linear color
     *
     * Represent linear RGBA color, encoded with float values;
     */
    class Color {
    public:

        using R8 = uint8;
        using RGBA8 = uint32;
        using ARGB8 = uint32;
        using ABGR8 = uint32;
        using BGRA8 = uint32;

        static const size_t ComponentsCount = 4;

        enum class Component {
            R = 0,
            G = 1,
            B = 2,
            A = 3
        };

        Color() = default;

        Color(float r) {
            mValues = Math::Vec4f(r, 0.0f, 0.0f, 1.0f);
        }

        Color(float r, float g, float b) {
            mValues = Math::Vec4f(r, g, b, 1.0f);
        }

        Color(float r, float g, float b, float a) {
            mValues = Math::Vec4f(r, g, b, a);
        }

        Color(const Math::Vec4f& vec) {
            mValues = vec;
        }

        Color operator+(const Color& other) const {
            return mValues + other.mValues;
        }

        Color operator-(const Color& other) const
        {
            return mValues - other.mValues;
        }

        Color operator*(float value) const
        {
            return mValues * value;
        }

        Color operator*(const Color& other) const
        {
            return mValues * other.mValues;
        }

        Color operator/(const Color& other) const
        {
            return mValues / other.mValues;
        }

        Color operator/(float value) const
        {
            BERSERK_ASSERT(value != 0.0f);
            return mValues / value;
        }

        Color& operator+=(const Color& other)
        {
            mValues += other.mValues;
            return *this;
        }

        Color& operator-=(const Color& other)
        {
            mValues -= other.mValues;
            return *this;
        }

        Color& operator*=(float value)
        {
            mValues *= value;
            return *this;
        }

        Color& operator/=(float value)
        {
            BERSERK_ASSERT(value != 0.0f);
            mValues /= value;
            return *this;
        }

        static Color FromR8    (R8    color);
        static Color FromRGBA8 (RGBA8 color);
        static Color FromARGB8 (ARGB8 color);
        static Color FromABGR8 (ABGR8 color);
        static Color FromBGRA8 (BGRA8 color);

        float* GetValues() { return mValues.values; }
        const float* GetValues() const { return mValues.values; }
        static size_t GetValuesCount() { return ComponentsCount; }

        float R() const { return mValues.values[(size_t) Component::R]; }
        float G() const { return mValues.values[(size_t) Component::G]; }
        float B() const { return mValues.values[(size_t) Component::B]; }
        float A() const { return mValues.values[(size_t) Component::A]; }

        float& R() { return mValues.values[(size_t) Component::R]; }
        float& G() { return mValues.values[(size_t) Component::G]; }
        float& B() { return mValues.values[(size_t) Component::B]; }
        float& A() { return mValues.values[(size_t) Component::A]; }

    private:
        Math::Vec4f mValues;
    };

}

#endif //BERSERK_COLOR_HPP
//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_STATICSTRING_H
#define BERSERK_STATICSTRING_H

#include "Public/Strings/StringUtility.h"

namespace Berserk
{

    template <typename T, T end, uint32 size>
    class StaticString
    {
    public:

        typedef T CharType;
        typedef Strings<T, end> Utils;

        /** Template factory for strings with fixed different sizes */
        static const uint32 STRING_SIZE = size;

    public:

        StaticString();

        StaticString(const CharType* source);

        StaticString(const StaticString& source);

        StaticString operator = (const StaticString& string);

        StaticString operator + (const StaticString& string) const;

        StaticString operator += (const StaticString& string);

        const bool operator >= (const StaticString& string) const;

        const bool operator <= (const StaticString& string) const;

        const bool operator == (const StaticString& string) const;

        uint32 getLength() const;

        T* get();

    private:

        CharType mBuffer[STRING_SIZE];

    };

    template <typename T, T end, uint32 size>
    StaticString<T, end, size>::StaticString()
    {
        mBuffer[0] = end;
    }

    template <typename T, T end, uint32 size>
    StaticString<T, end, size>::StaticString(const CharType *source) : StaticString()
    {
        Utils::strncat(mBuffer, source, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    StaticString<T, end, size>::StaticString(const StaticString &source)
    {
        Utils::strcpy(mBuffer, source.mBuffer);
    }

    template <typename T, T end, uint32 size>
    StaticString<T, end, size> StaticString<T, end, size>::operator=(const StaticString &string)
    {
        Utils::strcpy(mBuffer, string.mBuffer);
    }

    template <typename T, T end, uint32 size>
    StaticString<T, end, size> StaticString<T, end, size>::operator+(const StaticString &string) const
    {
        StaticString result = *this;
        Utils::strncat(result.mBuffer, string.mBuffer, STRING_SIZE);

        return result;
    }

    template <typename T, T end, uint32 size>
    StaticString<T, end, size> StaticString<T, end, size>::operator+=(const StaticString &string)
    {
        Utils::strncat(mBuffer, string.mBuffer, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    const bool StaticString<T, end, size>::operator<=(const StaticString &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) <= 0);
    }

    template <typename T, T end, uint32 size>
    const bool StaticString<T, end, size>::operator>=(const StaticString &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) >= 0);
    }

    template <typename T, T end, uint32 size>
    const bool StaticString<T, end, size>::operator==(const StaticString &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) == 0);
    }

    template <typename T, T end, uint32 size>
    uint32 StaticString<T, end, size>::getLength() const
    {
        return Utils::strlen(mBuffer);
    }

    template <typename T, T end, uint32 size>
    T* StaticString<T, end, size>::get()
    {
        return mBuffer;
    }

} // namespace Berserk

#endif //BERSERK_STATICSTRING_H

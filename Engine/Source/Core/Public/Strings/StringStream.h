//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_STRINGSTREAM_H
#define BERSERK_STRINGSTREAM_H

#include "Public/Strings/StringUtility.h"

namespace Berserk
{

    template <typename T, T end, uint32 size>
    class StringStream
    {
    public:

        typedef T CharType;
        typedef Strings<T, end> Utils;

        /** Template factory for strings with fixed different sizes */
        static const uint32 STRING_SIZE = size;

    public:

        StringStream();

        StringStream(const CharType* source);

        StringStream(const StringStream& source);

        void empty();

        void insert(const StringStream& source, uint32 pos);

        StringStream operator = (const StringStream& string);

        StringStream operator + (const StringStream& string) const;

        StringStream operator += (const StringStream& string);

        StringStream operator += (const char* string);

        const bool operator >= (const StringStream& string) const;

        const bool operator <= (const StringStream& string) const;

        const bool operator == (const StringStream& string) const;

        uint32 length() const;

        T* get();

    private:

        CharType mBuffer[STRING_SIZE];

    };

    template <typename T, T end, uint32 size>
    StringStream<T, end, size>::StringStream()
    {
        mBuffer[0] = end;
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size>::StringStream(const CharType *source) : StringStream()
    {
        Utils::strncat(mBuffer, source, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size>::StringStream(const StringStream &source)
    {
        Utils::strcpy(mBuffer, source.mBuffer);
    }

    template <typename T, T end, uint32 size>
    void StringStream<T, end, size>::empty()
    {
        mBuffer[0] = end;
    }

    template <typename T, T end, uint32 size>
    void StringStream<T, end, size>::insert(const StringStream &source, uint32 pos)
    {
        Utils::strnins(mBuffer, source.mBuffer, pos, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator=(const StringStream &string)
    {
        Utils::strcpy(mBuffer, string.mBuffer);
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator+(const StringStream &string) const
    {
        StringStream result = *this;
        Utils::strncat(result.mBuffer, string.mBuffer, STRING_SIZE);

        return result;
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator+=(const StringStream &string)
    {
        Utils::strncat(mBuffer, string.mBuffer, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator+=(const char *string)
    {
        Utils::strncat(mBuffer, string, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    const bool StringStream<T, end, size>::operator<=(const StringStream &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) <= 0);
    }

    template <typename T, T end, uint32 size>
    const bool StringStream<T, end, size>::operator>=(const StringStream &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) >= 0);
    }

    template <typename T, T end, uint32 size>
    const bool StringStream<T, end, size>::operator==(const StringStream &string) const
    {
        return (Utils::strcmp(mBuffer, string.mBuffer) == 0);
    }

    template <typename T, T end, uint32 size>
    uint32 StringStream<T, end, size>::length() const
    {
        return Utils::strlen(mBuffer);
    }

    template <typename T, T end, uint32 size>
    T* StringStream<T, end, size>::get()
    {
        return mBuffer;
    }

} // namespace Berserk

#endif //BERSERK_STRINGSTREAM_H

//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_STRINGSTREAM_H
#define BERSERK_STRINGSTREAM_H

#include "Misc/Crc32.h"
#include "Strings/StringUtility.h"

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

        StringStream(const StringStream& source);

        explicit StringStream(const CharType* source);

        void empty();

        void insert(const StringStream& source, uint32 pos);

        StringStream replace(const StringStream& what, const StringStream& source);

        StringStream operator = (const StringStream& string);

        StringStream operator = (const T *string);

        StringStream operator + (const StringStream& string) const;

        void operator += (const StringStream& string);

        void operator += (const T* string);

        const bool operator >= (const StringStream& string) const;

        const bool operator <= (const StringStream& string) const;

        const bool operator == (const StringStream& string) const;

        const bool operator == (const T* string) const;

        uint32 length() const;

        uint32 hash() const { return Hashing(mBuffer); }

        const T* get() const;

        static uint32 Hashing(const void* key)
        {
            auto string = (StringStream<T,end,size>*)key;
            auto len = string->length();
            return Crc32::hash(string->get(), len);
        }

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
        mBuffer[0] = end;
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
    StringStream<T, end, size> StringStream<T, end, size>::replace(const StringStream &what, const StringStream &source)
    {
        auto target_length = Utils::strlen(mBuffer);
        auto what_length = Utils::strlen(what.mBuffer);
        auto source_length = Utils::strlen(source.mBuffer);

        if (target_length - what_length + source_length >= size)
        {
            return *this;
        }

        auto i = Utils::strstr(mBuffer, what.mBuffer);

        if (i == -1)
        {
            return *this;
        }

        StringStream result;

        {
            Utils::strncpy(result.mBuffer, mBuffer, (uint32)i);
            Utils::strncpy((T*)(result.mBuffer) + (uint32)i, source.mBuffer, source_length);
            Utils::strncpy((T*)(result.mBuffer) + source_length + (uint32)i,
                           (T*)mBuffer + what_length + (uint32)i,
                           target_length - what_length - (uint32)i + 1);
        }

        return result;
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator=(const StringStream &string)
    {
        Utils::strcpy(mBuffer, string.mBuffer);
        return *this;
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator=(const T *string)
    {
        mBuffer[0] = end;
        Utils::strncat(mBuffer, string, STRING_SIZE);

        return *this;
    }

    template <typename T, T end, uint32 size>
    StringStream<T, end, size> StringStream<T, end, size>::operator+(const StringStream &string) const
    {
        StringStream result = *this;
        Utils::strncat(result.mBuffer, string.mBuffer, STRING_SIZE);

        return result;
    }

    template <typename T, T end, uint32 size>
    void StringStream<T, end, size>::operator+=(const StringStream &string)
    {
        Utils::strncat(mBuffer, string.mBuffer, STRING_SIZE);
    }

    template <typename T, T end, uint32 size>
    void StringStream<T, end, size>::operator+=(const T *string)
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
    const bool StringStream<T, end, size>::operator==(const T* string) const
    {
        return (Utils::strcmp(mBuffer, string) == 0);
    }

    template <typename T, T end, uint32 size>
    uint32 StringStream<T, end, size>::length() const
    {
        return Utils::strlen(mBuffer);
    }

    template <typename T, T end, uint32 size>
    const T* StringStream<T, end, size>::get() const
    {
        return mBuffer;
    }

} // namespace Berserk

#endif //BERSERK_STRINGSTREAM_H

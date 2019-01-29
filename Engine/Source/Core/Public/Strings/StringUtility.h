//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_STRINGUTILITY_H
#define BERSERK_STRINGUTILITY_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Misc/Include.h"
#include "Public/Misc/Buffers.h"

namespace Berserk
{

    /**
     * Generic base substitute for default C string functionality
     * @tparam T    Character type, which specifies stream symbol type
     * @tparam end  Symbol to mark the end of the string
     */
    template <typename T, T end> class Strings
    {
    public:

        typedef T CharType;

        /**
         * Cannot provide strins function for streams with
         * size more than 4 KiB
         */
        static const uint32 MAX_BYTE_STREAM_SIZE = Buffers::KiB * 4;

    public:

        static uint32 strlen(const CharType* source);

        static int32 strstr(const CharType *source, const CharType *substring);

        static int32 strcmp(const CharType* str1, const CharType* str2);

        static int32 strncmp(const CharType* str1, const CharType* str2, uint32 size);

        static void strcpy(CharType* destination, const CharType* source);

        static void strncpy(CharType* destination, const CharType* source, uint32 size);

        static void strcat(CharType* destination, const CharType* source);

        static void strncat(CharType* destination, const CharType* source, uint32 size);

        static void strins(CharType* destination, const CharType* source, uint32 position);
    };

    template <typename T, T end>
    uint32 Strings<T, end>::strlen(const CharType *source)
    {
        uint32 len = 0;
        while (*source != end)
        {
            len += 1;
            source += 1;
        }

        return len;
    }

    template <typename T, T end>
    int32 Strings<T, end>::strstr(const CharType *source, const CharType *substring)
    {
        int32 position = 0;

        while (*source != end)
        {
            auto src = source;
            auto sub = substring;

            while (*src == *sub && *sub != end && *src != end)
            {
                src += 1;
                sub += 1;
            }

            if (*sub == end)
            {
                return position;
            }

            position += 1;
            source += 1;
        }

        return (-1);
    }

    template <typename T, T end>
    int32 Strings<T, end>::strcmp(const CharType *str1, const CharType *str2)
    {
        while (*str1 == *str2 && *str1 != end && *str2 != end)
        {
            str1 += 1;
            str2 += 1;
        }

        return (*str1 - *str2);
    }

    template <typename T, T end>
    int32 Strings<T, end>::strncmp(const CharType *str1, const CharType *str2, uint32 size)
    {
        int32 count = size;

        while (*str1 == *str2 && *str1 != end && *str2 != end && count > 1)
        {
            str1 += 1;
            str2 += 1;
            count -= 1;
        }

        return (*str1 - *str2);
    }

    template <typename T, T end>
    void Strings<T, end>::strcpy(CharType *destination, const CharType *source)
    {
        while (*source != end)
        {
            *destination = *source;
            destination += 1;
            source += 1;
        }

        *destination = *source;
    }

    template <typename T, T end>
    void Strings<T, end>::strncpy(CharType *destination, const CharType *source, uint32 size)
    {
        memcpy(destination, source, size * sizeof(CharType));
    }

    template <typename T, T end>
    void Strings<T, end>::strcat(CharType *destination, const CharType *source)
    {
        while (*destination != end)
        {
            destination += 1;
        }

        while (*source != end)
        {
            *destination = *source;
            destination += 1;
            source += 1;
        }

        *destination = *source;
    }

    template <typename T, T end>
    void Strings<T, end>::strncat(CharType *destination, const CharType *source, uint32 size)
    {
        int32 len = 1; // we explicitly insert 'end' symbol in the end
        while (*destination != end && len < size)
        {
            destination += 1;
            len += 1;
        }

        while (*source != end && len < size)
        {
            *destination = *source;
            destination += 1;
            source += 1;
            len += 1;
        }

        *destination = end;
    }

    template <typename T, T end>
    void Strings<T, end>::strins(CharType *destination, const CharType *source, uint32 position)
    {
        auto source_len = Strings::strlen(source);
        auto destination_len = Strings::strlen(destination);

        if (position == destination_len)
        {
            Strings::strcat(destination, source);
        }
        else if (position > destination_len)
        {
            Strings::strcpy(destination + position, source);
        }
        else
        {
            ASSERT(destination_len + source_len < MAX_BYTE_STREAM_SIZE,
                   "Final stream size must be less than %u in the str insert function", MAX_BYTE_STREAM_SIZE);

            CharType* buffer = buffer6;

            Strings::strncpy(buffer, destination + position, destination_len - position + 1);
            Strings::strcpy(destination + position, source);
            Strings::strcat(destination, buffer);
        }

    }

} // namespace Berserk

#endif //BERSERK_STRINGUTILITY_H

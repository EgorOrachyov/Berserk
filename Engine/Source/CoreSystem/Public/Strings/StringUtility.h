//
// Created by Egor Orachyov on 29.01.2019.
//

#ifndef BERSERK_STRINGUTILITY_H
#define BERSERK_STRINGUTILITY_H

#include <Misc/Crc32.h>
#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Misc/Buffers.h>
#include <Math/MathUtility.h>

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
         * Cannot provide strings function for streams with
         * size more than 4 KiB
         */
        static const uint32 MAX_BYTE_STREAM_SIZE = Buffers::KiB * 4;

    public:

        /**
         * @param source Target string
         * @return Length of the string source without 'end' symbol
         */
        static uint32 strlen(const CharType* source);

        /**
         * @param source    Target string
         * @param substring String to be found in the source
         * @return Index of first symbol, which belongs to the substring in string
         *         source, or -1 if it is not found
         */
        static int32 strstr(const CharType *source, const CharType *substring);

        static int32 strcmp(const CharType* str1, const CharType* str2);

        /**
         *
         * @param str1 String to compare
         * @param str2 String to compare
         * @param size Compare only first size bytes of strings
         * @return (-,0,+) in lexicographical order for strings str1 and srt2
         */
        static int32 strncmp(const CharType* str1, const CharType* str2, uint32 size);

        static void strcpy(CharType* destination, const CharType* source);

        /**
         *
         * @param destination Target string
         * @param source      String to be copied
         * @param size        Num of symbols to copy
         * @warning 'end' symbol won't be placed explicitly
         */
        static void strncpy(CharType* destination, const CharType* source, uint32 size);

        static void strcat(CharType* destination, const CharType* source);

        /**
         *
         * @param destination Target string
         * @param source      String to append in the end of the destination string
         * @param size        Size of the resulted string (note, that 'end' will
         *                    be placed explicitly, therefore size could be reduced
         *                    to size - 1 automatically)
         */
        static void strncat(CharType* destination, const CharType* source, uint32 size);

        /**
         * @warning destination + source lengths must be less than MAX_BYTE_STREAM_SIZE
         * @param destination Target string
         * @param source      String to insert
         * @param position    Num of the symbol, after that source will be inserted
         */
        static void strins(CharType* destination, const CharType* source, uint32 position);

        static void strnins(CharType* destination, const CharType* source, uint32 position, uint32 size);

        static uint32 Hashing(const void* key)
        {
            auto string = (T*)key;
            auto len = strlen(string);
            return Crc32::hash(string, len);
        }
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
            return;
        }
        else
        {
            if (destination_len + source_len >= MAX_BYTE_STREAM_SIZE)
            {
                sprintf(buffer1, "Final stream size must be less than %u in the str insert function\n", MAX_BYTE_STREAM_SIZE);
                sprintf(buffer2, "(LINE %li: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1);
                fprintf(stderr,  "%s", buffer2);
                exit(EXIT_FAILURE);
            }

            CharType* buffer = buffer6;

            Strings::strncpy(buffer, destination + position, destination_len - position + 1);
            Strings::strcpy(destination + position, source);
            Strings::strcat(destination, buffer);
        }

    }

    template <typename T, T end>
    void Strings<T, end>::strnins(CharType *destination, const CharType *source, uint32 position, uint32 size)
    {
        auto source_len = Strings::strlen(source);
        auto destination_len = Strings::strlen(destination);

        if (position == destination_len)
        {
            Strings::strncat(destination, source, size);
        }
        else if (position > destination_len)
        {
            return;
        }
        else
        {
            if (destination_len + source_len >= MAX_BYTE_STREAM_SIZE)
            {
                sprintf(buffer1, "Final stream size must be less than %u in the str insert function\n", MAX_BYTE_STREAM_SIZE);
                sprintf(buffer2, "(LINE %li: FUNCTION %s: FILE %s)\n > %s", __LINE__, __FUNCTION__, __FILE__, buffer1);
                fprintf(stderr,  "%s", buffer2);
                exit(EXIT_FAILURE);
            }

            CharType* buffer = buffer6;

            Strings::strncpy(buffer, destination + position, destination_len - position + 1);

            Strings::strncpy(destination + position, source, (uint32) Math::min((int32)(source_len + 1), Math::max((int32)0, (int32)size - (int32)position)));
            destination[size - 1] = end;

            Strings::strncat(destination, buffer, size);
            destination[size - 1] = end;
        }

    }

} // namespace Berserk

#endif //BERSERK_STRINGUTILITY_H

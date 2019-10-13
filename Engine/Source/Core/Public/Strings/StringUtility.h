//
// Created by Egor Orachyov on 25.06.2019.
//

#ifndef BERSERK_STRINGUTILITY_H
#define BERSERK_STRINGUTILITY_H

#include <HAL/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * Generic utility functions for working with strings
     * @note Thread-Safe, no internal memory allocations
     * @note end symbol is explicitly inserted in the end of the each destination string
     *
     * @tparam Char Type of characters in string
     * @tparam end End symbol (where the string actually ends)
     */
    template <typename Char, Char end>
    class ENGINE_API StringUtility
    {
    public:

        /**
         * @param source String buffer
         * @return Lenght of the string source
         */
        static uint32 length(const Char* source)
        {
            uint32 length = 0;
            while (*source != end)
            {
                source += 1;
                length += 1;
            }

            return length;
        }

        /**
         * Copy from source to destination string
         * @param destination String buffer where store result
         * @param source String buffer with the source
         */
        static void copy(Char* destination, const Char* source)
        {
            while (*source != end)
            {
                *destination = *source;
                destination += 1;
                source += 1;
            }

            /** Insert end symbol */
            *destination = end;
        }

        /**
         * Copy from source to destination string
         * @param destination String buffer where store result
         * @param size Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         */
        static void copy(Char* destination, uint32 size, const Char* source)
        {
            uint32 written = 0;
            while (*source != end && written < size - 1)
            {
                *destination = *source;
                destination += 1;
                source += 1;
                written += 1;
            }

            /** Insert end symbol */
            *destination = end;
        }

        /**
         * Concatenation of destination and source strings
         * @param destination String buffer where store result
         * @param source String buffer with the source
         */
        static void concat(Char* destination, const Char* source)
        {
            uint32 l = length(destination);
            copy(destination + l, source);
        }

        /**
         * Concatenation of destination and source strings
         * @param destination String buffer where store result
         * @param size Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         */
        static void concat(Char* destination, uint32 size, const Char* source)
        {
            uint32 l = length(destination);
            copy(destination + l, size - l, source);
        }

        /**
         * Compares to strings  in lexicographical order for strings str1 and srt2
         * @param str1 Source to compare
         * @param str2 Source to compare
         * @return (-,0,+) if str1 < str2, str1 == str2, str1 > str2
         */
        static int32 compare(const Char* str1, const Char* str2)
        {
            while (*str1 == *str2 && *str1 != end && *str2 != end)
            {
                str1 += 1;
                str2 += 1;
            }

            return (*str1 - *str2);
        }

        /**
         * Compares to strings  in lexicographical order for strings str1 and srt2
         * @param str1 Source to compare
         * @param str2 Source to compare
         * @param count Total number of chars to compare
         * @return (-,0,+) if str1 < str2, str1 == str2, str1 > str2
         */
        static int32 compare(const Char* str1, const Char* str2, uint32 count)
        {
            while (*str1 == *str2 && *str1 != end && *str2 != end && count > 1)
            {
                str1 += 1;
                str2 += 1;
                count -= 1;
            }

            return (*str1 - *str2);
        }

        /**
         * Copy count chars from source string
         * @param destination String buffer where store result
         * @param source String buffer with the source
         * @param from Index of the first char to be copied
         * @param count Count of char to copy
         */
        static void substring(Char* destination, const Char* source, uint32 from, uint32 count)
        {
            source += from;
            while (*source != end && count > 0)
            {
                *destination = *source;
                source += 1;
                destination += 1;
                count -= 1;
            }

            /** Insert end symbol explicitly */
            *destination = end;
        }

        /**
         * Copy count chars from source string
         * @param destination String buffer where store result
         * @param size Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         * @param from Index of the first char to be copied
         * @param count Count of char to copy
         */
        static void substring(Char* destination, uint32 size, const Char* source, uint32 from, uint32 count)
        {
            source += from;
            while (*source != end && count > 0 && size > 1)
            {
                *destination = *source;
                source += 1;
                destination += 1;
                count -= 1;
                size -= 1;
            }

            /** Insert end symbol explicitly */
            *destination = end;
        }

        /**
         * Returns pointer to the first char of the substring in string source
         * @param source String buffer with the source
         * @param substring String to find in the source
         * @return nullptr if there is nothing or
         *         pointer to first substring in source string
         */
        Char* find(const Char* source, const Char* substring)
        {
            if (*source == end || *substring == end)
            {
                return nullptr;
            }

            while (*source != end)
            {
                if (*source == *substring)
                {
                    Char* s = source + 1;
                    Char* f = substring + 1;

                    while (*s == *f && s != end && f != end)
                    {
                        s += 1;
                        f += 1;
                    }

                    if (*f == end)
                    {
                        return source;
                    }
                }

                source += 1;
            }

            return nullptr;
        }

    };

} // namespace Berserk

#endif //BERSERK_STRINGUTILITY_H
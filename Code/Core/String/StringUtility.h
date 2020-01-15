/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STRINGUTILITY_H
#define BERSERK_STRINGUTILITY_H

#include <Typedefs.h>
#include <Platform/Memory.h>

namespace Berserk {

    /**
    * Generic utility functions for working with zero-terminated strings
    * @note Thread-Safe, no internal memory allocations
    * @note end symbol is explicitly inserted in the end of the each destination string
    *
    * @tparam Char Type of characters in string
    * @tparam end End symbol (where the string actually ends)
    */
    template <typename Char = char, Char end = '\0'>
    class StringUtility
    {
    public:

        /**
         * @param source String buffer
         * @return Length of the string source without '\0'
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
         * @param destSize Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         */
        static void copy(Char* destination, uint32 destSize, const Char* source)
        {
            uint32 written = 0;
            while (*source != end && written < destSize - 1)
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
         * @param destSize Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         */
        static void concat(Char* destination, uint32 destSize, const Char* source)
        {
            uint32 l = length(destination);
            copy(destination + l, destSize - l, source);
        }

        /**
         * Compares two strings  in lexicographical order for strings str1 and srt2
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
         * Compares two strings in lexicographical order for strings str1 and srt2
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
        static const Char* findFirst(const Char *source, const Char *substring)
        {
            if (*source == end || *substring == end)
            {
                return nullptr;
            }

            while (*source != end)
            {
                if (*source == *substring)
                {
                    const Char* s = source;
                    const Char* f = substring;

                    while (*s == *f && *s != end && *f != end)
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

        /**
         * Replace first substring what in string source to replacement and write result in destination string
         * @param destination Buffer to store final string
         * @param source String where replace first string
         * @param what Substring to be replaced
         * @param replacement Replacement string (inserted instead of what string)
         */
        static void replaceFirst(Char* destination, const Char* source, const char* what, const char* replacement) {
            const Char* where = findFirst(source, what);

            if (where != nullptr) {
                uint32 r = length(replacement);
                uint32 w = length(what);
                uint32 offset = (uint32)(where - source);

                Memory::copy(destination, source, offset);
                destination += offset;
                Memory::copy(destination, replacement, r);
                destination += r;
                source += offset + w;

                copy(destination, source);
            }
        }
    };

}

#endif //BERSERK_STRINGUTILITY_H
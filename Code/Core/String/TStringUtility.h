/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TSTRINGUTILITY_H
#define BERSERK_TSTRINGUTILITY_H

#include <Math/Math.h>

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
    class TStringUtility
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
        static void copy(Char* destination, int32 destSize, const Char* source)
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
        static void concat(Char* destination, int32 destSize, const Char* source)
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
         * Returns pointer to the first substring in string source
         * @param source String buffer with the source
         * @param what String to find in the source
         * @return nullptr if there is nothing or
         *         pointer to first substring in source string
         */
        static const Char* findFirst(const Char *source, const Char *what)
        {
            if (*source == end || *what == end)
            {
                return nullptr;
            }

            while (*source != end)
            {
                if (*source == *what)
                {
                    const Char* s = source;
                    const Char* f = what;

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
         * Returns pointer to the last substring in string source
         * @param source String buffer with the source
         * @param what String to find in the source
         * @return nullptr if there is nothing or
         *         pointer to last substring in source string
         */
        static const Char* findLast(const Char* source, const Char* what) {
            auto offset = length(source);

            auto p = source + offset;
            while (p != source) {
                if (*p == *what) {
                    const Char* s = p;
                    const Char* f = what;

                    while (*s == *f && *s != end && *f != end)
                    {
                        s += 1;
                        f += 1;
                    }

                    if (*f == end)
                    {
                        return p;
                    }
                }

                p -= 1;
            }

            return nullptr;
        }

        /**
         * Replaces count symbols from the first symbol in string source to the
         * string replacement and writes result to the destination
         *
         * @warning first+count substring must be (valid) inside of source string
         *
         * @param destination Buffer to write result
         * @param destSize Capacity of the buffer
         * @param first Index of the first symbol to replace
         * @param count Number of symbols to be replaced
         * @param replacement Replacement string
         */
        static void replace(Char* destination, int32 destSize, const Char* source, uint32 first, uint32 count, const Char* replacement) {
            auto pos = source + first;
            while (destSize > 1 && source != pos) {
                *destination = *source;
                source += 1;
                destination += 1;
                destSize -= 1;
            }

            while (destSize > 1 && *replacement != end) {
                *destination = *replacement;
                replacement += 1;
                destination += 1;
                destSize -= 1;
            }

            source += count;

            while (destSize > 1 && *source != end) {
                *destination = *source;
                source += 1;
                destination += 1;
                destSize -= 1;
            }

            *destination = end;
        }

        /**
         * Return substring offset inside source string
         * @param source Source string
         * @param substring A substring of the source (must be valid)
         * @return Index offset
         */
        static int32 offset(const Char* source, const Char* substring) {
            return (int32)(substring - source);
        }

        /**
         * Repeats what string N times and stores result in destination string
         * @param destination Buffer to write result
         * @param destSize Capacity of the buffer
         * @param what String t repeat
         * @param N Times
         */
        static void repeat(Char* destination, int32 destSize, const Char* what, int32 N) {
            while (destSize > 1 && N > 0) {
                auto p = what;
                while (destSize > 1 && *p != end) {
                    *destination = *p;
                    destination += 1;
                    p += 1;
                    destSize -= 1;
                }
                N -= 1;
            }

            *destination = end;
        }

    };

}

#endif //BERSERK_TSTRINGUTILITY_H
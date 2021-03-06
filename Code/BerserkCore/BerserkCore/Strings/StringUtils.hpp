/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_STRINGUTILS_HPP
#define BERSERK_STRINGUTILS_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    /**
    * Generic utility functions for working with zero-terminated strings
    * @note Thread-Safe, no internal memory allocations
    * @note end symbol is explicitly inserted in the end of the each destination string
    *
    * @tparam Char Type of characters in string
    * @tparam end End symbol (where the string actually ends)
    */
    template<typename Char = char, Char end = '\0'>
    class TStringUtils {
    public:

        /**
         * @param source String buffer
         * @return Length of the string source without end
         */
        static uint32 Length(const Char *source) {
            uint32 length = 0;
            while (*source != end) {
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
        static void Copy(Char *destination, const Char *source) {
            while (*source != end) {
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
        static void Copy(Char *destination, int32 destSize, const Char *source) {
            uint32 written = 0;
            while (*source != end && written < destSize - 1) {
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
        static void Concat(Char *destination, const Char *source) {
            uint32 l = Length(destination);
            Copy(destination + l, source);
        }

        /**
         * Concatenation of destination and source strings
         * @param destination String buffer where store result
         * @param destSize Total size of the destination buffer, where we actually can write
         * @param source String buffer with the source
         */
        static void Concat(Char *destination, int32 destSize, const Char *source) {
            uint32 l = Length(destination);
            Copy(destination + l, destSize - l, source);
        }

        /**
         * Compares two strings  in lexicographical order for strings str1 and srt2
         * @param str1 Source to compare
         * @param str2 Source to compare
         * @return (-,0,+) if str1 < str2, str1 == str2, str1 > str2
         */
        static int32 Compare(const Char *str1, const Char *str2) {
            while (*str1 == *str2 && *str1 != end && *str2 != end) {
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
        static int32 Compare(const Char *str1, const Char *str2, uint32 count) {
            while (*str1 == *str2 && *str1 != end && *str2 != end && count > 1) {
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
         * @param from Index of the first Char to be copied
         * @param count Count of Char to copy
         */
        static void Substring(Char *destination, const Char *source, uint32 from, uint32 count) {
            source += from;
            while (*source != end && count > 0) {
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
         * @param from Index of the first Char to be copied
         * @param count Count of Char to copy
         */
        static void Substring(Char *destination, uint32 size, const Char *source, uint32 from, uint32 count) {
            source += from;
            while (*source != end && count > 0 && size > 1) {
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
         * Returns true if source string starts with provided prefix
         * @param source Source string to check
         * @param prefix Prefix string to check
         * @return True if source = prefix + rest
         */
        static bool IfStarts(const Char *source, const Char *prefix) {
            while (*source != end && *prefix != end && *source == *prefix) {
                source += 1;
                prefix += 1;
            }

            return *prefix == end;
        }

        static bool IfEnds(const Char *source, const Char *suffix) {
            auto sl = Length(source);
            auto sffl = Length(suffix);

            auto currs = source - 1 + sl;
            auto currsff = suffix - 1 + sffl;

            while (sl != 0 && sffl != 0 && *currs == *currsff) {
                sl -= 1;
                sffl -= 1;
                currs -= 1;
                currsff -= 1;
            }

            return sffl == 0;
        }

        /**
         * Returns pointer to the first substring in string source
         * @param source String buffer with the source
         * @param what String to find in the source
         * @return nullptr if there is nothing or
         *         pointer to first substring in source string
         */
        static const Char *FindFirst(const Char *source, const Char *what) {
            if (*source == end || *what == end) {
                return nullptr;
            }

            while (*source != end) {
                if (*source == *what) {
                    const Char *s = source;
                    const Char *f = what;

                    while (*s == *f && *s != end && *f != end) {
                        s += 1;
                        f += 1;
                    }

                    if (*f == end) {
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
        static const Char *FindLast(const Char *source, const Char *what) {
            auto offset = Length(source);

            auto p = source + offset;
            while (p != source) {
                if (*p == *what) {
                    const Char *s = p;
                    const Char *f = what;

                    while (*s == *f && *s != end && *f != end) {
                        s += 1;
                        f += 1;
                    }

                    if (*f == end) {
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
        static void Replace(Char *destination, int32 destSize, const Char *source, uint32 first, uint32 count,
                            const Char *replacement) {
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
        static int32 OffsetOf(const Char *source, const Char *substring) {
            return (int32) (substring - source);
        }

        /**
         * Repeats what string N times and stores result in destination string
         * @param destination Buffer to write result
         * @param destSize Capacity of the buffer
         * @param what String t repeat
         * @param N Times
         */
        static void Repeat(Char *destination, int32 destSize, const Char *what, int32 N) {
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

        /**
         * Copy to destination string source data before suffix
         * @example Destination equals 'abb' if source 'abb/cd' and suffix '/cd'
         * @param destination Where to store result
         * @param source Source string to copy data before suffix
         * @param suffix Suffix to skip
         */
        static void CopyBeforeSuffix(Char *destination, const Char *source, const Char *suffix) {
            while (source != suffix) {
                *destination = *source;

                destination += 1;
                source += 1;
            }

            *destination = end;
        }

        /**
         * Removes specified symbols from string
         * @param destination String to store result
         * @param source Source string
         * @param symbols Null terminated symbols to remove
         * @return Number of the symbols removed
         */
        static uint32 RemoveSymbols(Char *destination, const Char *source, const Char *symbols) {
            uint32 removed = 0;

            while (*source != end) {
                const Char *iterate = symbols;

                while (*iterate != end && *source != *iterate) {
                    iterate += 1;
                }

                if (*iterate == end) {
                    *destination = *source;
                    destination += 1;
                } else {
                    removed += 1;
                }

                source += 1;
            }

            *destination = '\0';

            return removed;
        }

    };

    using StringUtils = TStringUtils<char, '\0'>;
    using WStringUtils = TStringUtils<wchar, L'\0'>;

}

#endif //BERSERK_STRINGUTILS_HPP
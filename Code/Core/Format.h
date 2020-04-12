/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FORMAT_H
#define BERSERK_FORMAT_H

#include <Typedefs.h>
#include <type_traits>
#include <stdio.h>

namespace Berserk {

    class Format {
    private:

        template <typename T>
        static constexpr bool isCharStringPtr() {
            return std::is_pointer<T>::value &&
                   std::is_same<typename std::remove_cv<typename std::remove_pointer<T>::type>::type, char>::value;
        }

        template <typename T>
        static constexpr bool isCharStringExt() {
            return std::is_array<T>::value && std::is_same<typename std::remove_extent<T>::type, char>::value;
        }

        template <typename T>
        static constexpr bool isCharString() {
            return isCharStringExt<T>() || isCharStringPtr<T>();
        }

    public:

        /**
         * Raw C print wrapper for printing in char buffer of limited size.
         * If buffer != null and size == 0 then buffer data will be null-terminated.
         *
         * @tparam TArgs template arguments to print (with no type check)
         * @param buffer Char buffer to store result
         * @param size Buffer size (where we could write data)
         * @param format Formatting string (%f, %s, %u, %i, etc.)
         * @param args Arguments to print
         * @return number of char written (without '\0' symbol)
         */
        template <typename ... TArgs>
        static uint32 printf(char *buffer, uint32 size, const char *format, TArgs&& ... args) {
            if (!buffer || (size == 0)) {
                return 0;
            }

            int32 result = snprintf(buffer, size, format, std::forward<TArgs>(args)...);

            if (result < 0) {
                return 0;
            } else if (result < size) {
                return result;
            } else {
                return size - 1;
            }
        }

    };

}

#endif //BERSERK_FORMAT_H
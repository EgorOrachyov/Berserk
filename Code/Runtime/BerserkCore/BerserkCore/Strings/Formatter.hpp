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

#ifndef BERSERK_FORMATTER_HPP
#define BERSERK_FORMATTER_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringBuilder.hpp>
#include <BerserkCore/Templates/Array.hpp>

namespace Berserk {

    /**
     * @brief Print formatter
     *
     * Type safe generic text printing formatter.
     * Replacement for standard unsafe printf functionality.
     *
     * This formatter caches stream and internal args array. That allows to employ
     * the formatter for several print operations and allows to reduce the
     * number of memory allocations. But all cached resources can be freed
     * only when the formatter is destructed.
     *
     * @tparam Stream Generic stream, used to build result formatted string
     * @tparam Alloc Allocator, used to for internal memory allocations
     */
    template<typename Stream = StringBuilder, typename Alloc = Allocator>
    class Formatter {
    public:

        /**
         * Configure formatter with stream, used to build actual string data
         * @param stream Generic stream instance, stored inside formatter
         * @param alloc Alloc instance, stored inside formatter
         */
        explicit Formatter(Stream&& stream = StringBuilder(), Alloc&& alloc = Allocator())
            : mStream(std::move(stream)), mPrintedArgs(std::move(alloc)) {

        }

        /**
         * Print formatted with provided string message "{0} {1} ... " with
         * arbitrary set of arguments, which are indexed with their positions.
         *
         * @tparam TArgs Type of generic args to print
         * @param source Format string
         * @param args Actual args to print
         *
         * @return Formatted string
         */
        template<typename ... TArgs>
        String Print(const String& source, TArgs&& ... args) {
            return std::move(Print(source.GetLength(), source.GetStr_C(), std::forward<TArgs>(args)...));
        }

        /**
         * Print formatted with provided string message "{0} {1} ... " with
         * arbitrary set of arguments, which are indexed with their positions.
         *
         * @tparam TArgs Type of generic args to print
         * @param source Format string (must be null terminated)
         * @param args Actual args to print
         *
         * @return Formatted string
         */
        template<typename ... TArgs>
        String Print(const String::Char8u* source, TArgs&& ... args) {
            return std::move(Print(String::Utils::Length(source), source, std::forward<TArgs>(args)...));
        }

        /**
         * Print formatted with provided string message "{0} {1} ... " with
         * arbitrary set of arguments, which are indexed with their positions.
         *
         * @tparam TArgs Type of generic args to print
         * @param sourceLength Length of the source string in basic CharType units
         * @param source Format string
         * @param args Actual args to print
         *
         * @return Formatted string
         */
        template<typename ... TArgs>
        String Print(uint64 sourceLength, const String::Char8u* source, TArgs&& ... args) {
            // Collect args into single array, uses the same indices, as in the format
            mPrintedArgs.Clear();
            ArgsCollector<void, TArgs...>::Collect(mPrintedArgs, mStream, std::forward<TArgs>(args)...);

            mStream.Clear();

            bool escaped = false;
            uint32 i = 0;
            while (i < sourceLength) {
                switch (source[i]) {
                    case '{': {
                        if (escaped) {
                            escaped = false;
                            mStream.Add(source[i]);
                            i += 1;

                            break;
                        }

                        i += 1;
                        const String::Char8u* start = &source[i];
                        uint32 length = 0;

                        while (i < sourceLength && isDigit(source[i]) && source[i] != '}') {
                            i += 1;
                            length += 1;
                        }

                        if (source[i] != '}') {
                            // todo: exception
                            return "";
                        }

                        if (length > 0) {
                            uint32 index = String(start, length).ToUint32();

                            if (index >= mPrintedArgs.GetSize()) {
                                // todo: exception
                                return "";
                            }

                            const auto& arg = mPrintedArgs[index];
                            mStream.Add(arg);
                        }

                        i += 1;
                    } break;

                    case '}': {
                        if (escaped) {
                            mStream.Add(source[i]);
                            i += 1;
                        }
                        else {
                            // todo: exception
                            return "";
                        }
                    }break;

                    case '\\': {
                        if (!escaped) {
                            escaped = true;
                        }
                        else {
                            mStream.Add(source[i]);
                            escaped = false;
                        }

                        i += 1;
                    }break;

                    default: {
                        mStream.Add(source[i]);
                        i += 1;
                        escaped = false;
                    }
                }
            }

            mPrintedArgs.Clear();

            return std::move(mStream.ToString());
        }

    private:

        // Print all args into strings and place inside array
        template<typename D, typename ... TArgs>
        struct ArgsCollector {};

        template<typename D>
        struct ArgsCollector<D> {
            static void Collect(Array<String, Alloc>& printedArgs, Stream& stream) {
                // no op
            }
        };

        template<typename D, typename T>
        struct ArgsCollector<D, T> {
            static void Collect(Array<String, Alloc>& printedArgs, Stream& stream, T&& arg) {
                stream.Clear();
                TextPrint<typename std::remove_const<typename std::remove_reference<T>::type>::type> formatPrint;
                formatPrint(stream, std::forward<T>(arg));
                printedArgs.Emplace(std::move(stream.ToString()));
            }
        };

        template<typename D, typename T, typename ... TArgs>
        struct ArgsCollector<D, T, TArgs...> {
            static void Collect(Array<String, Alloc>& printedArgs, Stream& stream, T&& arg, TArgs&& ... args) {
                stream.Clear();
                TextPrint<typename std::remove_const<typename std::remove_reference<T>::type>::type> formatPrint;
                formatPrint(stream, std::forward<T>(arg));
                printedArgs.Emplace(std::move(stream.ToString()));
                ArgsCollector<D, TArgs...>::Collect(printedArgs, stream, std::forward<TArgs>(args)...);
            }
        };

        static bool isDigit(String::Char8u symbol) {
            return symbol >= '0' && symbol <= '9';
        }

        /** Internal stream for strings building */
        Stream mStream;

        /** Array to collect args */
        Array<String, Alloc> mPrintedArgs;
    };


    /** Common format elements */

    template <>
    class TextPrint<int16> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, int16 value) const {
            stream.Add(String::Fromi32(value));
        }
    };

    template <>
    class TextPrint<int32> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, int32 value) const {
            stream.Add(String::Fromi32(value));
        }
    };

    template <>
    class TextPrint<int64> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, int64 value) const {
            stream.Add(String::Fromi64(value));
        }
    };

    template <>
    class TextPrint<uint16> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, uint16 value) const {
            stream.Add(String::Fromi32(value));
        }
    };

    template <>
    class TextPrint<uint32> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, uint32 value) const {
            stream.Add(String::Fromu32(value));
        }
    };

    template <>
    class TextPrint<uint64> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, uint64 value) const {
            stream.Add(String::Fromu64(value));
        }
    };

    template <>
    class TextPrint<float> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, float value) const {
            stream.Add(String::Fromf(value));
        }
    };

    template <>
    class TextPrint<double> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, double value) const {
            stream.Add(String::Fromd(value));
        }
    };

    template <>
    class TextPrint<bool> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, bool value) const {
            stream.Add(String::Fromb(value));
        }
    };

    template <uint64 Size>
    class TextPrint<const char[Size]> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const char value[Size]) const {
            stream.Add(value);
        }
    };

    template <uint64 Size>
    class TextPrint<char[Size]> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const char value[Size]) const {
            stream.Add(value);
        }
    };

    template <>
    class TextPrint<const char*> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const char* value) const {
            stream.Add(value);
        }
    };

    template <typename T>
    class Precision {
    public:
        Precision(T value, uint32 precision)
            : value(value), precision(precision) {

        }

        T value;
        uint32 precision;
    };

    template <typename T>
    class TextPrint<Precision<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, Precision<T>&& fmt) const {
            stream.Add(String::Fromf(fmt.value, fmt.precision));
        }
    };

    template <typename T>
    class TextPrint<T*> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const T* ptr) const {
            stream.Add(String::Fromp((const void *) ptr));
        }
    };

    template <typename T>
    class TextPrint {
    public:
        template<typename Stream>
        void operator()(Stream& stream, T&& t) const {
            stream.Add(String::From(std::forward<T>(t)));
        }
    };

}

#endif //BERSERK_FORMATTER_HPP
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FORMAT_HPP
#define BERSERK_FORMAT_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Misc/Contracts.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringBuilder.hpp>
#include <BerserkCore/Containers/Array.hpp>

namespace Berserk {

    /**
     * @brief Print formatter
     *
     * Type safe generic text printing formatter.
     * Replacement for standard unsafe printf functionality.
     *
     * @tparam Stream Generic stream, used to build result formatted string
     * @tparam Alloc Allocator, used to for internal memory allocations
     */
    template<typename Stream = StringBuilder, typename Alloc = Platform::Allocator>
    class Format {
    public:

        /**
         * Configure formatter with stream, used to build actual string data
         * @param stream Generic stream instance, stored inside formatter
         * @param alloc Alloc instance, stored inside formatter
         */
        explicit Format(Stream&& stream = StringBuilder(), Alloc&& alloc = Platform::Allocator())
            : mStream(std::move(stream)), mAlloc(std::move(alloc)) {

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
        String format(const String& source, TArgs&& ... args) {
            return std::move(format(source.GetLength(), source.GetStr(), std::forward<TArgs>(args)...));
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
        String format(const String::CharType* source, TArgs&& ... args) {
            return std::move(format(String::Utils::Length(source), source, std::forward<TArgs>(args)...));
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
        String format(size_t sourceLength, const String::CharType* source, TArgs&& ... args) {
            // Collect args into single array, uses the same indices, as in the format
            Array<String, Alloc> printedArgs(mAlloc);
            ArgsCollector<void, TArgs...>::Collect(printedArgs, mStream, std::forward<TArgs>(args)...);

            mStream.Clear();

            bool escaped = false;
            size_t i = 0;
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
                        const String::CharType* start = &source[i];
                        size_t length = 0;

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

                            if (index >= printedArgs.GetSize()) {
                                // todo: exception
                                return "";
                            }

                            const auto& arg = printedArgs[index];
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
                FormatPrint<typename std::remove_reference<T>::type> formatPrint;
                formatPrint(stream, std::forward<T>(arg));
                printedArgs.Emplace(std::move(stream.ToString()));
            }
        };

        template<typename D, typename T, typename ... TArgs>
        struct ArgsCollector<D, T, TArgs...> {
            static void Collect(Array<String, Alloc>& printedArgs, Stream& stream, T&& arg, TArgs&& ... args) {
                stream.Clear();
                FormatPrint<typename std::remove_reference<T>::type> formatPrint;
                formatPrint(stream, std::forward<T>(arg));
                printedArgs.Emplace(std::move(stream.ToString()));
                ArgsCollector<D, TArgs...>::Collect(printedArgs, stream, std::forward<TArgs>(args)...);
            }
        };

        static bool isDigit(String::CharType symbol) {
            return symbol >= '0' && symbol <= '9';
        }

        /** Internal stream for strings building */
        Stream mStream;

        /** Allocator for internal usage */
        Alloc mAlloc;
    };


    /** Common format elements */

    template <>
    class FormatPrint<int32> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, int32 value) const {
            stream.Add(String::From(value));
        }
    };

    template <>
    class FormatPrint<int64> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, int64 value) const {
            stream.Add(String::From(value));
        }
    };

    template <>
    class FormatPrint<uint32> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, uint32 value) const {
            stream.Add(String::From(value));
        }
    };

    template <>
    class FormatPrint<uint64> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, uint64 value) const {
            stream.Add(String::From(value));
        }
    };

    template <>
    class FormatPrint<float> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, float value) const {
            stream.Add(String::From(value));
        }
    };

    template <>
    class FormatPrint<double> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, double value) const {
            stream.Add(String::From(value));
        }
    };

    template <size_t Size>
    class FormatPrint<const char[Size]> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const char value[Size]) const {
            stream.Add(value);
        }
    };

    template <typename T>
    class FormatPrint {
    public:
        template<typename Stream>
        void operator()(Stream& stream, T&& value) const {
            stream.Add(std::forward<T>(value));
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
    class FormatPrint<Precision<T>> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, Precision<T>&& fmt) const {
            stream.Add(String::From(fmt.value, fmt.precision));
        }
    };

}

#endif //BERSERK_FORMAT_HPP
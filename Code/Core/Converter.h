/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONVERTER_H
#define BERSERK_CONVERTER_H

#include <Typedefs.h>
#include <Math/Math.h>

namespace Berserk {

    /**
     * @brief Type converter/reader
     *
     * Converts decimal string streams to floating and integer values and vise versa.
     * Supports only decimal base for values input and output.
     */
    class Converter {
    public:

        static bool isDigit(char symbol) {
            return symbol >= '0' && symbol <= '9';
        }

        template <typename T>
        static const char* stringToSignedT(const char* stream, T &value) {
            T sign = 1;

            if (*stream == '-') {
                sign = -1;
                stream += 1;
            }

            T result = 0;

            while (*stream != '\0' && isDigit(*stream)) {
                T symbol = (*stream) - '0';
                result = result * 10 + symbol;
                stream += 1;
            }

            value = result * sign;
            return stream;
        }

        template <typename T>
        static const char* stringToUnsignedT(const char* stream, T &value) {
            T result = 0;

            while (*stream != '\0' && isDigit(*stream)) {
                T symbol = (*stream) - '0';
                result = result * 10 + symbol;
                stream += 1;
            }

            value = result;
            return stream;
        }

        template <typename T>
        static const char* streamToFloatingT(const char* stream, T &value) {
            T sign = 1.0f;

            if (*stream == '-') {
                sign = -1.0f;
                stream += 1;
            }

            int64 integerPart = 0;

            while (*stream != '\0' && isDigit(*stream)) {
                int64 symbol = (*stream) - '0';
                integerPart = integerPart * 10 + symbol;
                stream += 1;
            }

            T fractionalPart = 0.0f;
            T divider = 10.f;

            // Skip dot
            stream += 1;

            while (*stream != '\0' && isDigit(*stream)) {
                int64 symbol = (*stream) - '0';
                fractionalPart += ((T)symbol) / divider;
                divider *= 10.0f;
                stream += 1;
            }

            value = ((float)integerPart + fractionalPart) * (sign);
            return stream;
        }

        static const char* stringToInt32(const char* stream, int32 &value) {
            return stringToSignedT<int32>(stream, value);
        }

        static const char* stringToInt64(const char* stream, int64 &value) {
            return stringToSignedT<int64>(stream, value);
        }

        static const char* stringToUint32(const char* stream, uint32 &value) {
            return stringToUnsignedT<uint32>(stream, value);
        }

        static const char* stringToUint64(const char* stream, uint64 &value) {
            return stringToUnsignedT<uint64>(stream, value);
        }

        static const char* stringToFloat(const char *stream, float &value) {
            return streamToFloatingT<float>(stream, value);
        }

        static const char* stringToDouble(const char *stream, double &value) {
            return streamToFloatingT<double>(stream, value);
        }

        static char* int32ToString(char* stream, int32 value) {
            return stream + sprintf(stream, "%i", value);
        }

        static char* int64ToString(char* stream, int64 value) {
            return stream + sprintf(stream, "%lli", value);
        }

        static char* uint32ToString(char* stream, uint32 value) {
            return stream + sprintf(stream, "%u", value);
        }

        static char* uint64ToString(char* stream, uint64 value) {
            return stream + sprintf(stream, "%llu", value);
        }

        static char* floatToString(char *stream, float value, int32 precision = 4) {
            return stream + sprintf(stream, "%.*f", precision, value);
        }

        static char* doubleToString(char *stream, double value, int32 precision = 4) {
            return stream + sprintf(stream, "%.*lf", precision, value);
        }

    };

}

#endif //BERSERK_CONVERTER_H
/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
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

#ifndef BERSERK_PRINTERUTILS_H
#define BERSERK_PRINTERUTILS_H

#include <HAL/Platform.h>
#include <HAL/Memory.h>
#include <stdio.h>

namespace Berserk {

    /**
     * Printer utility class, wraps printf functionality for
     * formatted char and wide char string output
     */
    class PrinterUtils {
    public:

        /**
         * Wrapper for printf function
         * @tparam TArgs Types of args
         * @param buffer Actual storage for printed message
         * @param format String which specifies where to insert args
         * @param args Actual arguments to insert i formatted string
         * @return Count of chars successfully written
         */
        template<typename ... TArgs>
        static int32 print(char *buffer, const char *format, TArgs &&... args) {
            return sprintf(buffer, format, std::forward<TArgs>(args) ...);
        }

        /**
         * Wrapper for printf function
         * @tparam TArgs Types of args
         * @param buffer Actual storage for printed message
         * @param bufferSize Size of buffer to write result
         * @param format String which specifies where to insert args
         * @param args Actual arguments to insert i formatted string
         * @return Count of chars successfully written
         */
        template<typename ... TArgs>
        static int32 print(char *buffer, uint32 bufferSize, const char *format, TArgs &&... args) {
            return snprintf(buffer, bufferSize, format, std::forward<TArgs>(args) ...);
        }

       /**
        * Pretty bytes count printing
        * @param bytes Memory bytes to print
        * @param buffer Actual buffer to write data
        * @return Count of chars successfully written
        */
        static int32 print(uint32 bytes, char *buffer) {
            if (bytes / Memory::KiB == 0) {
                return sprintf(buffer, "%5u Byte", bytes);
            } else if (bytes / Memory::MiB == 0) {
                return sprintf(buffer, "%5.2f KiBs", (float32) bytes / (float32) Memory::KiB);
            } else if (bytes / Memory::GiB == 0) {
                return sprintf(buffer, "%5.2f MiBs", (float32) bytes / (float32) Memory::MiB);
            } else {
                return sprintf(buffer, "%5.2f GiBs", (float32) bytes / (float32) Memory::GiB);
            }
        }

        /**
        * Pretty bytes count printing
        * @param bytes Memory bytes to print
        * @param buffer Actual buffer to write data
        * @param align Length of the written count of bytes
        * @return Count of chars successfully written
        */
        static int32 print(uint32 bytes, uint32 align, char *buffer) {
            if (bytes / Memory::KiB == 0) {
                return sprintf(buffer, "%*u Byte", align, bytes);
            } else if (bytes / Memory::MiB == 0) {
                return sprintf(buffer, "%*.2f KiBs", align, (float32) bytes / (float32) Memory::KiB);
            } else if (bytes / Memory::GiB == 0) {
                return sprintf(buffer, "%*.2f MiBs", align, (float32) bytes / (float32) Memory::MiB);
            } else {
                return sprintf(buffer, "%*.2f GiBs", align, (float32) bytes / (float32) Memory::GiB);
            }
        }

    };

} // namespace Berserk

#endif //BERSERK_PRINTERUTILS_H
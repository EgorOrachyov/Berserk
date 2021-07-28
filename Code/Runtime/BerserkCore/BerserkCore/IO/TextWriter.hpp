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

#ifndef BERSERK_TEXTWRITER_HPP
#define BERSERK_TEXTWRITER_HPP

#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/Formatter.hpp>

namespace Berserk {

    /** Generic output device for text output in utf-8 format */
    class TextWriter {
    public:
        virtual ~TextWriter() = default;

        /**
         * Writes specified number of char type symbol into writer
         * @param symbolsCount Number of symbols in the buffer to write
         * @param string String of char type symbols
         */
        virtual void Write(uint64 symbolsCount, const String::Char8u* string) = 0;

        /**
         * Flushes content of the writer.
         * This method is automatically invoked on writer destruction.
         */
        virtual void Flush() = 0;

        virtual void Write(const String& string) {
            Write(string.GetLength(), string.GetStr_C());
        }

        virtual void Write(const String::Char8u* string) {
            Write(String::Utils::Length(string), string);
        }

        virtual void Write(int32 value) {
            Write(String::Fromi32(value));
        }

        virtual void Write(uint32 value) {
            Write(String::Fromu32(value));
        }

        virtual void Write(int64 value) {
            Write(String::Fromi64(value));
        }

        virtual void Write(uint64 value) {
            Write(String::Fromu64(value));
        }

        virtual void Write(float value) {
            Write(String::Fromf(value));
        }

        virtual void Write(double value) {
            Write(String::Fromd(value));
        }

        virtual void WriteLine() {
            Write(1, "\n");
        }

        virtual void WriteLine(const String& string) {
            Write(string.GetLength(), string.GetStr_C());
            WriteLine();
        }

        virtual void WriteLine(const String::Char8u* string) {
            Write(String::Utils::Length(string), string);
            WriteLine();
        }

        virtual void WriteLine(int32 value) {
            Write(String::Fromi32(value));
            WriteLine();
        }

        virtual void WriteLine(uint32 value) {
            Write(String::Fromu32(value));
            WriteLine();
        }

        virtual void WriteLine(int64 value) {
            Write(String::Fromi64(value));
            WriteLine();
        }

        virtual void WriteLine(uint64 value) {
            Write(String::Fromu64(value));
            WriteLine();
        }

        virtual void WriteLine(float value) {
            Write(String::Fromf(value));
            WriteLine();
        }

        virtual void WriteLine(double value) {
            Write(String::Fromd(value));
            WriteLine();
        }

        template<typename Source, typename ... TArgs>
        void WriteF(const Source& source, TArgs&& ... args) {
            Formatter<> formatter;
            Write(formatter.Print(source, std::forward<TArgs>(args)...));
        }

        template<typename Stream, typename Alloc, typename Source, typename ... TArgs>
        void WriteFF(Formatter<Stream,Alloc>& formatter, const Source& source, TArgs&& ... args) {
            Write(formatter.Print(source, std::forward<TArgs>(args)...));
        }

        template<typename T>
        friend TextWriter& operator<<(TextWriter& textWriter, const T& value) {
            textWriter.Write(value);
            return textWriter;
        }
    };

}

#endif //BERSERK_TEXTWRITER_HPP
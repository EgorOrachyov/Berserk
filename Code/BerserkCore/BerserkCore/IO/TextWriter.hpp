/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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
        virtual void Write(size_t symbolsCount, const String::CharType* string) = 0;

        /**
         * Flushes content of the writer.
         * This method is automatically invoked on writer destruction.
         */
        virtual void Flush() = 0;

        virtual void WriteLine() {
            Write(1, "\n");
        }

        virtual void Write(const String& string) {
            Write(string.GetLength(), string.GetStr());
        }

        virtual void Write(const String::CharType* string) {
            Write(String::Utils::Length(string), string);
        }

        virtual void Write(int32 value) {
            Write(String::From(value));
        }

        virtual void Write(uint32 value) {
            Write(String::From(value));
        }

        virtual void Write(int64 value) {
            Write(String::From(value));
        }

        virtual void Write(uint64 value) {
            Write(String::From(value));
        }

        virtual void Write(float value) {
            Write(String::From(value));
        }

        virtual void Write(double value) {
            Write(String::From(value));
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
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

#ifndef BERSERK_LOG_HPP
#define BERSERK_LOG_HPP

#include <BerserkCore/Platform/DateTime.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringName.hpp>
#include <BerserkCore/Strings/Formatter.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {

    /**
     * @brief Logger
     *
     * Log for logging messages. Allows to log messages with different categories and verbosity.
     * Provides utility to store message in text or html form.
     * Allows to observe logger messages to be notified when new entry is added.
     */
    class Log {
    public:

        enum class SaveFormat {
            /** Allows to save log as raw utf-8 text file */
            Text,
            /** Allows to save log as html file */
            Html
        };

        enum class Verbosity {
            /** Info about fatal error before crash or something similar */
            Fatal = 0,
            /** Info about error, but no crash, can skip or ignore */
            Error = 1,
            /** Warning info about something */
            Warning = 2,
            /** Info for the user */
            Info = 3,
            /** The same as Verbosity::Info, but does not shown in the user console */
            Logging = 4
        };

        class Entry {
        public:
            explicit Entry(StringName&& category, String&& message, Verbosity verbosity);

            /** What about this message */
            StringName category;
            /** Actual description */
            String message;
            /** Verbosity of this message */
            Verbosity verbosity = Verbosity::Logging;
            /** When it was logged */
            TimeStamp timeStamp;
        };

        /**
         * Log message with specified properties into log
         *
         * @param category Message category
         * @param message Actual message content
         * @param verbosity Message verbosity
         */
        void LogMessage(StringName&& category, String&& message, Verbosity verbosity);

        /**
         * Log formatted message with specified properties into log
         *
         * @tparam Source Type of the format string
         * @tparam TArgs Types of passed arguments to format
         *
         * @param category Message category
         * @param verbosity Message verbosity
         * @param source Format string
         * @param args Arguments to place inside format
         */
        template<typename Source, typename ... TArgs>
        void LogMessageF(StringName&& category, Verbosity verbosity, const Source& source, TArgs&& ... args) {
            Guard<RecursiveMutex> guard(mMutex);
            auto message = mFormatter.Print(source, std::forward<TArgs>(args)...);
            LogMessage(std::move(category), std::move(message), verbosity);
        }

        /** @return Copy of logged entries */
        void GetEntries(Array<Entry> &entries) const;

        /**
         * Allows to save log content in the specified format
         *
         * @param path Path to the file where to save log
         * @param saveFormat Target log format to save
         */
        void SaveLog(const String& path, SaveFormat saveFormat) const;

        /** @return Verbosity as string */
        static String GetVerbosityAsString(Verbosity verbosity);

    private:
        void SaveLogText(const String& path) const;
        void SaveLogHtml(const String& path) const;

        Array<Entry> mEntries;
        Formatter<> mFormatter;
        mutable RecursiveMutex mMutex;
    };


    template<>
    class TextPrint<Log::Verbosity> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, Log::Verbosity verbosity) const {
            stream.Add(Log::GetVerbosityAsString(verbosity));
        }
    };

}

#endif //BERSERK_LOG_HPP
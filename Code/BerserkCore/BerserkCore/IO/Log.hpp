/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOG_HPP
#define BERSERK_LOG_HPP

#include <BerserkCore/Misc/DateTime.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/Formatter.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/Threading/Synchronization.hpp>

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
            explicit Entry(String&& category, String&& message, Verbosity verbosity);

            /** What about this message */
            String category;                            // todo: hashed string name
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
        void LogMessage(String&& category, String&& message, Verbosity verbosity);

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
        void LogMessageF(String&& category, Verbosity verbosity, const Source& source, TArgs&& ... args) {
            Guard<RecursiveMutex> guard(mMutex);
            auto message = mFormatter.Print(source, std::forward<TArgs>(args)...);
            LogMessage(std::move(category), std::move(message), verbosity);
        }

        /** @return Copy of logged entries */
        void GetEntries(Array<Entry> &entries) const;

    private:
        Array<Entry> mEntries;
        Formatter<> mFormatter;
        mutable RecursiveMutex mMutex;
    };

    template<>
    class FormatPrint<Log::Verbosity> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, Log::Verbosity verbosity) const {
            static const char* VerbosityToText[] = {
                BERSERK_TEXT("Fatal"),
                BERSERK_TEXT("Error"),
                BERSERK_TEXT("Warning"),
                BERSERK_TEXT("Info"),
                BERSERK_TEXT("Logging")
            };

            auto id = (uint32)verbosity;

            if (id <= (uint32)Log::Verbosity::Logging) {
                stream.Add(VerbosityToText[id]);
            }
            else {
                stream.Add(BERSERK_TEXT("Unknown"));
            }
        }
    };

}

#endif //BERSERK_LOG_HPP
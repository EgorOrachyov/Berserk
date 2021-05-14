/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/IO/Log.hpp>
#include <BerserkCore/IO/FileTextWriter.hpp>
#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    Log::Entry::Entry(StringName &&category, String &&message, Verbosity verbosity)
        : category(std::move(category)), message(std::move(message)), verbosity(verbosity), timeStamp(System::GetTimeStamp()) {

    }

    void Log::LogMessage(StringName &&category, String &&message, Verbosity verbosity) {
        Guard<RecursiveMutex> guard(mMutex);
        auto& entry = mEntries.Emplace(std::move(category), std::move(message), verbosity);

        auto date = Date();
        auto time = Time();
        System::GetDateTime(entry.timeStamp, date, time, TimeType::Local);

        auto formatted = mFormatter.Print(BERSERK_TEXT("[{0}.{1}.{2} {3}:{4}:{5}][{6}] {7}: {8}\n"),
                                          date.year, date.GetMonthNumber(), date.dayMonth,
                                          time.hour, time.min, time.sec,
                                          entry.verbosity, entry.category, entry.message);

        if (verbosity == Verbosity::Error || verbosity == Verbosity::Fatal) {
            auto& err = System::Error();
            err.Write(formatted);
        }

        else if (verbosity == Verbosity::Warning || verbosity == Verbosity::Info) {
            auto& out = System::Out();
            out.Write(formatted);
        }
    }

    void Log::GetEntries(Array<Entry> &entries) const {
        Guard<RecursiveMutex> guard(mMutex);
        entries = mEntries;
    }

    void Log::SaveLog(const String &path, SaveFormat saveFormat) const {
        switch (saveFormat) {
            case SaveFormat::Text:
                SaveLogText(path);
                return;
            case SaveFormat::Html:
                SaveLogHtml(path);
                return;
            default:
                // todo: log error
                return;
        }
    }

    String Log::GetVerbosityAsString(Verbosity verbosity) {
        const char* VerbosityToText[] = {
                BERSERK_TEXT("Fatal"),
                BERSERK_TEXT("Error"),
                BERSERK_TEXT("Warning"),
                BERSERK_TEXT("Info"),
                BERSERK_TEXT("Logging")
        };

        auto id = (uint32)verbosity;

        if (id <= (uint32)Log::Verbosity::Logging) {
            return VerbosityToText[id];
        }

        return BERSERK_TEXT("Unknown");
    }

    void Log::SaveLogText(const String &path) const {
        FileTextWriter fileTextWriter(path);
        Array<Entry> entries;

        GetEntries(entries);

        auto date = System::GetDate(TimeType::Local);
        auto time = System::GetTime(TimeType::Local);

        fileTextWriter << BERSERK_TEXT("BERSERK Engine (c) Log file\n")
                       << BERSERK_TEXT("Formed at ") << date.ToString()
                       << BERSERK_TEXT(" ") << time.ToString() << BERSERK_TEXT("\n");

        StringBuilder messageBuilder;

        for (const auto& entry: entries) {
            auto messageDate = Date();
            auto messageTime = Time();
            System::GetDateTime(entry.timeStamp, messageDate, messageTime, TimeType::Local);

            messageBuilder
                .Add(BERSERK_TEXT("["))
                .Add(messageDate.ToString())
                .Add(BERSERK_TEXT(" "))
                .Add(messageTime.ToString())
                .Add(BERSERK_TEXT("]["))
                .Add(GetVerbosityAsString(entry.verbosity))
                .Add(BERSERK_TEXT("] "))
                .Add(entry.category)
                .Add(BERSERK_TEXT(":"))
                .Add(entry.message);

            fileTextWriter.Write(messageBuilder.ToString());
            fileTextWriter.WriteLine();

            messageBuilder.Clear();
        }
    }

    const char* GetRowColorFromMessageType(Log::Verbosity verbosity) {
        static const char ColorLogging[] = "#F5F5F5";
        static const char ColorInfo[] = "#FFFFFF";
        static const char ColorWarning[] = "#FFE6CC";
        static const char ColorError[] = "#F8CECC";
        static const char ColorFatal[] = "#FF6969";

        switch (verbosity) {
            case Log::Verbosity::Fatal:
                return ColorFatal;
            case Log::Verbosity::Error:
                return ColorError;
            case Log::Verbosity::Warning:
                return ColorWarning;
            case Log::Verbosity::Info:
                return ColorInfo;
            default:
                return ColorLogging;
        }
    }

    void Log::SaveLogHtml(const String &path) const {
        FileTextWriter fileTextWriter(path);
        Array<Entry> entries;

        GetEntries(entries);

        auto date = System::GetDate(TimeType::Local);
        auto time = System::GetTime(TimeType::Local);

        const char preStyleHeader[] =
            BERSERK_TEXT(
                "<!DOCTYPE html>\n"
                "<html>"
            );

        const char tableStyle[] =
            BERSERK_TEXT(
                "<head>\n"
                " <style>\n"
                "  table {\n"
                "   font-family: arial, sans-serif;\n"
                "   border-collapse: collapse;\n"
                "   width: 100%;\n"
                "  }\n"
                "  td, th {\n"
                "   border: 1px solid #dddddd;\n"
                "   text-align: left;\n"
                "   padding: 8px;\n"
                "  }\n"
                " </style>\n"
                "</head>\n"
            );

        const char postStyleHeader[] =
            BERSERK_TEXT(
                "<body>\n"
                "<h2>BERSERK Engine (c) Log file</h2>\n"
            );

        const char tableHeader[] =
            BERSERK_TEXT(
                "<table>\n"
                "<tr>\n"
                " <th style=\"width: 5%\">Message Number</th>\n"
                " <th style=\"width: 8%\">Message Time</th>\n"
                " <th style=\"width: 5%\">Message Type</th>\n"
                " <th style=\"width: 8%\">Message Tag</th>\n"
                " <th style=\"width: 74%\">Message</th>\n"
                "</tr>\n"
            );

        const char postTable[] =
            BERSERK_TEXT(
                "</table>\n"
                "</body>\n"
                "</html>\n"
            );

        fileTextWriter << preStyleHeader
                       << tableStyle
                       << postStyleHeader;

        fileTextWriter << BERSERK_TEXT("<h3>Formed at ")
                       << date.ToString()
                       << BERSERK_TEXT(" ")
                       << time.ToString()
                       << BERSERK_TEXT("</h3>\n");

        fileTextWriter << tableHeader;

        StringBuilder messageBuilder;
        size_t messageIndex = 0;

        for (const auto& entry: entries) {
            auto messageDate = Date();
            auto messageTime = Time();
            System::GetDateTime(entry.timeStamp, messageDate, messageTime, TimeType::Local);

            messageBuilder
                .Add(BERSERK_TEXT("<tr style=\"background-color: ")).Add(GetRowColorFromMessageType(entry.verbosity)).Add(BERSERK_TEXT("\">\n"))
                    .Add(BERSERK_TEXT("<td>")).Add(String::From(messageIndex)).Add(BERSERK_TEXT("</td>\n"))
                    .Add(BERSERK_TEXT("<td>")).Add(date.ToString()).Add(BERSERK_TEXT(" ")).Add(time.ToString()).Add(BERSERK_TEXT("</td>\n"))
                    .Add(BERSERK_TEXT("<td>")).Add(GetVerbosityAsString(entry.verbosity)).Add(BERSERK_TEXT("</td>\n"))
                    .Add(BERSERK_TEXT("<td>")).Add(entry.category).Add(BERSERK_TEXT("</td>\n"))
                    .Add(BERSERK_TEXT("<td>")).Add(entry.message).Add(BERSERK_TEXT("</td>\n"))
                .Add(BERSERK_TEXT("</tr>\n"));

            fileTextWriter.Write(messageBuilder.ToString());

            messageBuilder.Clear();
            messageIndex += 1;
        }

        fileTextWriter << postTable;
    }

}

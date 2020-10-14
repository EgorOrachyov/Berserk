/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/LogHtml.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {

    LogHtml::LogHtml(String pathToFile, Log &logToDump) {
        auto& system = Application::Get().GetSystem();
        auto file = system.OpenFile(std::move(pathToFile), EFileMode::Write);

        const char HTML_TABLE_STYLE[] =
            "<head>\n"
            "<style>\n"
            "table {\n"
            " font-family: arial, sans-serif;\n"
            " border-collapse: collapse;\n"
            " width: 100%;\n"
            "}\n"
            "td, th {\n"
            " border: 1px solid #dddddd;\n"
            " text-align: left;\n"
            " padding: 8px;\n"
            "}\n"
            "</style>\n"
            "</head>";

        const char HTMLTABLE_HEADER[] =
            "<tr>"
            " <th style=\"width: 8%\">Message Number</th>"
            " <th style=\"width: 9%\">Message Type</th>"
            " <th style=\"width: 13%\">Message Tag</th>"
            " <th style=\"width: 70%\">Message</th>"
            "</tr>";

        if (file) {
            mLogContent << "<!DOCTYPE html>\n";
            mLogContent << "<html>";
            mLogContent << HTML_TABLE_STYLE;
            mLogContent << "<body>";
            mLogContent << "<h2>Berserk Log</h2>";
            mLogContent << "<table>";
            mLogContent << HTMLTABLE_HEADER;
            logToDump.DumpMessages(*this);
            mLogContent << "</table>";
            mLogContent << "</body>";
            mLogContent << "</html>";

            auto& buffer = mLogContent.GetBuffer();
            file->Write(buffer.GetData(), buffer.GetSizeBytes());
        }
    }

    const char* GetRowColorFromMessageType(ELogMessageType messageType) {
        const char* COLORS[] = { "#F5F5F5", "#FFE6CC", "#F8CECC", "#FFFFFF" };
        const uint32 COLORS_COUNT = 4;

        auto index = (uint32) messageType;
        return index < COLORS_COUNT? COLORS[index]: COLORS[COLORS_COUNT - 1];
    }

    void LogHtml::OnEntry(uint32 index, ELogMessageType messageType, const char *messageTag, const char *message) {
        mLogContent << "<tr style=\"background-color: " << GetRowColorFromMessageType(messageType) << "\">";
        mLogContent << "<td>" << index << "</td>";
        mLogContent << "<td>" << Log::GetLogMessageTypeAsStr(messageType) << "</td>";
        mLogContent << "<td>" << messageTag << "</td>";
        mLogContent << "<td>" << message << "</td>";
        mLogContent << "</tr>";
    }

}

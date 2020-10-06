/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/LogHtmlDump.hpp>
#include <BerserkCore/Application.hpp>

/*

 <!DOCTYPE html>
<html>
<body>

<h2>Basic HTML Table</h2>

<table style="width:100%">
  <tr>
    <th>Firstname</th>
    <th>Lastname</th>
    <th>Age</th>
  </tr>
  <tr>
    <td>Jill</td>
    <td>Smith</td>
    <td>50</td>
  </tr>
  <tr>
    <td>Eve</td>
    <td>Jackson</td>
    <td>94</td>
  </tr>
  <tr>
    <td>John</td>
    <td>Doe</td>
    <td>80</td>
  </tr>
</table>

</body>
</html>

 */

namespace Berserk {

    LogHtmlDump::LogHtmlDump(String pathToFile, Log &logToDump) {
        if (System* system = Application::GetSingleton().GetSystem()) {
            auto file = system->OpenFile(std::move(pathToFile), EFileMode::Write);

            if (file) {
                mLogContent << "<!DOCTYPE html>\n";
                mLogContent << "<html>";
                mLogContent << "<body>";
                mLogContent << "<h2>Berserk HTML Log Dump</h2>";
                mLogContent << "<table style=\"width:100%\">";
                mLogContent << "<tr><th>Message Type</th><th>Message Tag</th><th>Message</th></tr>";
                logToDump.DumpMessages(*this);
                mLogContent << "</table>";
                mLogContent << "</body>";
                mLogContent << "</html>";

                auto& buffer = mLogContent.GetBuffer();
                file->Write(buffer.GetData(), buffer.GetSizeBytes());
            }
        }
    }

    void LogHtmlDump::OnEntry(uint32 index, ELogMessageType messageType, const char *messageTag, const char *message) {
        mLogContent << "<tr>";
        mLogContent << "<td>" << Details::GetLogMessageTypeAsStr(messageType) << "</td>";
        mLogContent << "<td>" << messageTag << "</td>";
        mLogContent << "<td>" << message << "</td>";
        mLogContent << "</tr>";
    }
}

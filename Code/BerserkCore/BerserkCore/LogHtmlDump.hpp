/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOGHTMLDUMP_HPP
#define BERSERK_LOGHTMLDUMP_HPP

#include <BerserkCore/Log.hpp>
#include <BerserkCore/String/StringBuilder.h>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    class LogHtmlDump: public LogDumpObserver {
    public:
        LogHtmlDump(String pathToFile, Log& logToDump);
        ~LogHtmlDump() override = default;

        void OnEntry(uint32 index, ELogMessageType messageType, const char *messageTag, const char *message) override;

    private:
        StringBuilder mLogContent;
        TPtrShared<File> mLogFile;
    };

}

#endif //BERSERK_LOGHTMLDUMP_HPP
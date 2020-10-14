/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOGHTML_HPP
#define BERSERK_LOGHTML_HPP

#include <BerserkCore/Log.hpp>
#include <BerserkCore/String/StringBuilder.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    class LogHtml: public LogVisitor {
    public:
        LogHtml(String pathToFile, Log& logToDump);
        ~LogHtml() override = default;

        void OnEntry(uint32 index, ELogMessageType messageType, const char *messageTag, const char *message) override;

    private:
        StringBuilder mLogContent;
        TPtrShared<File> mLogFile;
    };

}

#endif //BERSERK_LOGHTML_HPP
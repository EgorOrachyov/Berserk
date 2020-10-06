/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_PLATFORMLOG_HPP
#define BERSERK_PLATFORMLOG_HPP

#include <BerserkCore/Log.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/String/String.hpp>

namespace Berserk {

    class PlatformLog final : public Log {
    public:
        ~PlatformLog() override = default;
        void AddListener(LogListener &listener) override;
        void RemoveListener(LogListener &listener) override;
        void LogMessage(ELogMessageType messageType, const char *messageTag, const char *message) override;
        void LogMessage(ELogMessageType messageType, const char *messageTag, const char *message, uint32 messageLength) override;
        void DumpMessages(LogDumpObserver &observer) const override;

    private:
        void NotifyListeners(ELogMessageType messageType, const char *messageTag, const char *message);

        struct Entry {
            String mMessageTag;
            String mMessage;
            ELogMessageType mMessageType;
        };

        TArray<Entry> mLogEntries;
        TArray<LogListener*> mListeners;
    };
}

#endif //BERSERK_PLATFORMLOG_HPP
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/PlatformLog.hpp>

namespace Berserk {

    void PlatformLog::AddListener(LogListener &listener) {
        if (!mListeners.Contains(&listener))
            mListeners.Add(&listener);
    }

    void PlatformLog::RemoveListener(LogListener &listener) {
        mListeners.RemoveElement(&listener);
    }

    void PlatformLog::LogMessage(ELogMessageType messageType, const char *messageTag, const char *message) {
        Entry entry;
        entry.mMessageType = messageType;
        entry.mMessageTag = messageTag;
        entry.mMessage = message;

        NotifyListeners(messageType, messageTag, entry.mMessage.GetStr());

        mLogEntries.Move(entry);
    }

    void PlatformLog::LogMessage(ELogMessageType messageType, const char *messageTag, const char *message, uint32 messageLength) {
        Entry entry;
        entry.mMessageType = messageType;
        entry.mMessageTag = messageTag;
        entry.mMessage = std::move(String(message, messageLength));

        NotifyListeners(messageType, messageTag, entry.mMessage.GetStr());

        mLogEntries.Move(entry);
    }

    void PlatformLog::NotifyListeners(ELogMessageType messageType, const char *messageTag, const char *message) {
        for (auto listener: mListeners) {
            listener->OnLog(messageType, messageTag, message);
        }
    }

    void PlatformLog::DumpMessages(LogDumpObserver &observer) const {
        for (uint32 index = 0; index < mLogEntries.GetSize(); index++) {
            auto& entry = mLogEntries[index];
            observer.OnEntry(index, entry.mMessageType, entry.mMessageTag.GetStr(), entry.mMessage.GetStr());
        }
    }

}

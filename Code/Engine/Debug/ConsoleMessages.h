/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLEMESSAGES_H
#define BERSERK_CONSOLEMESSAGES_H

#include <IO/Log.h>
#include <IO/OutputDevice.h>
#include <String/WString.h>
#include <Platform/Mutex.h>
#include <Containers/TArray.h>

namespace Berserk {

    /** Caches at runtime messages from System logger to show in the console */
    class ConsoleMessages : public LogListener {
    public:

        ConsoleMessages();
        ~ConsoleMessages();

        /** Get recent messages */
        void getMessages(TArray<WString> &messages, TArray<EOutputType> &types) const;

        /** Get recent messages and clear buffer */
        void popMessages(TArray<WString> &messages, TArray<EOutputType> &types);

        /** Enable messages collecting */
        void setEnableCollection(bool enable);

        /** @return True if currently collecting messages (otherwise ignores new messages) */
        bool isCollectionEnabled() const;

        /** @return Count of the messages */
        uint32 getMessagesCount() const;

        /** Called when something is logged to the System log */
        void onLog(ELogType messageType, const char *message) override;

        /** @return Singleton instance */
        static ConsoleMessages& getSingleton();

    private:

        mutable Mutex mAccessMutex;
        bool mIsCollecting = true;
        TArray<WString> mMessages;
        TArray<EOutputType> mMessagesTypes;

        static ConsoleMessages* gMessages;
    };

}

#endif //BERSERK_CONSOLEMESSAGES_H
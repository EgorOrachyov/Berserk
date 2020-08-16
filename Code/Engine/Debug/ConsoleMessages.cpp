/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Debug/ConsoleMessages.h>
#include <Platform/System.h>

namespace Berserk {

    ConsoleMessages* ConsoleMessages::gMessages = nullptr;

    ConsoleMessages::ConsoleMessages() {
        BERSERK_COND_ERROR_RET(gMessages == nullptr, "Allowed only single ConsoleMessages instance");
        gMessages = this;

        auto& system = System::getSingleton();
        auto& log = system.getLog();

        if (log.supportsListeners()) {
            log.addListener(*this);
        }
    }

    ConsoleMessages::~ConsoleMessages() {
        auto& system = System::getSingleton();
        auto& log = system.getLog();

        if (log.supportsListeners()) {
            log.removeListener(*this);
        }

        gMessages = nullptr;
    }

    void ConsoleMessages::getMessages(TArray<WString> &messages, TArray<EOutputType> &types) const {
        Guard scopeLock(mAccessMutex);
        {
            messages.add(mMessages);
            types.add(mMessagesTypes);
        }
    }

    void ConsoleMessages::popMessages(TArray<WString> &messages, TArray<EOutputType> &types) {
        Guard scopeLock(mAccessMutex);
        {
            messages.add(mMessages);
            types.add(mMessagesTypes);
            mMessages.clear();
            mMessagesTypes.clear();
        }
    }

    void ConsoleMessages::setEnableCollection(bool enable) {
        Guard scopeLock(mAccessMutex);
        {
            mIsCollecting = enable;
        }
    }

    bool ConsoleMessages::isCollectionEnabled() const {
        Guard scopeLock(mAccessMutex);
        {
            bool ret = mIsCollecting;
            return ret;
        }
    }

    uint32 ConsoleMessages::getMessagesCount() const {
        Guard scopeLock(mAccessMutex);
        {
            uint32 count = mMessages.size();
            return count;
        }
    }

    void ConsoleMessages::onLog(ELogType messageType, const char *message) {
        Guard scopeLock(mAccessMutex);
        {
            if (mIsCollecting) {
                EOutputType outputType;

                switch (messageType) {
                    case ELogType::Info:
                        outputType = EOutputType::Info;
                        break;
                    case ELogType::Warning:
                        outputType = EOutputType::Warning;
                        break;
                    case ELogType::Error:
                        outputType = EOutputType::Error;
                        break;
                    default:
                        outputType = EOutputType::Text;
                        break;
                }

                auto time = System::getSingleton().getTime();
                auto timeInfo = time.toString();

                char buffer[2048];
                sprintf(buffer,"[%19s][%7s] %s",timeInfo.data(), Log::getLogTypeStringFromEnum(messageType), message);

                // Process multi-line text

                const char* start = buffer;
                uint32 position = 0;
                uint32 len = 0;

                while (buffer[position] != '\0') {
                    if (buffer[position] == '\n') {
                        WString text = std::move(WString::from(CString(start, len)));
                        mMessages.move(text);
                        mMessagesTypes.add(outputType);

                        start = &buffer[position + 1];
                        len = 0;
                    }

                    position += 1;
                    len += 1;
                }

                if (len > 0) {
                    WString text = std::move(WString::from(CString(start,len)));
                    mMessages.move(text);
                    mMessagesTypes.add(outputType);
                }
            }
        }
    }

    ConsoleMessages& ConsoleMessages::getSingleton() {
        return *gMessages;
    }

}
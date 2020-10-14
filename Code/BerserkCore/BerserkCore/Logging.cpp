/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Logging.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {
    void Logging::EngineLog(ELogMessageType messageType, const char* messageTag, const char* message, uint32 messageSize, bool abort) {
        auto& app = Application::Get();
        auto& log = app.GetLog();

        log.LogMessage(messageType, messageTag, message, messageSize);
        if (abort) app.Abort(message, messageSize);
    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/LogMacro.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {
    namespace Details {

        void EngineLogAlways(const char *messageTag, const char *message) {
            Application& gApp = Application::GetSingleton();

            if (Log* log = gApp.GetLog()) {
                log->LogMessage(ELogMessageType::Always, messageTag, message);
            }
        }

        void EngineLogInfo(const char *messageTag, const char *message) {
            Application& gApp = Application::GetSingleton();

            if (Log* log = gApp.GetLog()) {
                log->LogMessage(ELogMessageType::Info, messageTag, message);
            }
        }

        void EngineLogWarning(const char *messageTag, const char *message) {
            Application& gApp = Application::GetSingleton();

            // Notify System
            if (Log* log = gApp.GetLog()) {
                log->LogMessage(ELogMessageType::Warning, messageTag, message);
            }
        }

        void EngineLogError(const char *messageTag, const char *message) {
            Application& gApp = Application::GetSingleton();

            // Notify System
            if (Log* log = gApp.GetLog()) {
                log->LogMessage(ELogMessageType::Error, messageTag, message);
            }
            if (System* system = gApp.GetSystem()) {
                system->OnError(message);
            }
        }

        void EngineLogFatalError(const char *messageTag, const char *message) {
            Application& gApp = Application::GetSingleton();

            // Notify System (system must crash after this call)
            if (Log* log = gApp.GetLog()) {
                log->LogMessage(ELogMessageType::Error, messageTag, message);
            }
            if (System* system = gApp.GetSystem()) {
                system->OnFatalError(message);
            }
            // No return here
        }

    }
}
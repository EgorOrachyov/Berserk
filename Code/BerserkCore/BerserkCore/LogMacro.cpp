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

        void engineLogAlways(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();
            gApp.getLog().log(ELogMessageType::ALways, messageTag, message);
        }

        void engineLogInfo(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();
            gApp.getLog().log(ELogMessageType::Info, messageTag, message);
        }

        void engineLogWarning(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            // Notify System
            gApp.getLog().log(ELogMessageType::Warning, messageTag, message);
            gApp.getSystem().onWarning(message);
        }

        void engineLogError(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            // Notify System
            gApp.getLog().log(ELogMessageType::Error, messageTag, message);
            gApp.getSystem().onError(message);
        }

        void engineLogFatalError(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            // Notify System (system must crash after this call)
            gApp.getLog().log(ELogMessageType::Error, messageTag, message);
            gApp.getSystem().onFatalError(message);
            // No return here
        }

    }
}
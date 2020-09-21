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

        void engineOnInfo(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            gApp.getLog().log(ELogMessageType::Info, messageTag, message);
        }

        void engineOnWarning(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            gApp.getLog().log(ELogMessageType::Warning, messageTag, message);
            gApp.getSystem().onWarning(message);
        }

        void engineOnError(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            gApp.getLog().log(ELogMessageType::Info, messageTag, message);
            gApp.getSystem().onError(message);
        }

        void engineOnFatalError(const char *messageTag, const char *message) {
            Application& gApp = Application::getSingleton();

            gApp.getLog().log(ELogMessageType::Info, messageTag, message);
            gApp.getSystem().onFatalError(message);
        }

    }
}
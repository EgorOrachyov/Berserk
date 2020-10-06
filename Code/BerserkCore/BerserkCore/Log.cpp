/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Log.hpp>

namespace Berserk {
    namespace Details {
        const char* GetLogMessageTypeAsStr(ELogMessageType messageType) {
            switch (messageType) {
                case ELogMessageType::Always:
                    return "Always";
                case ELogMessageType::Info:
                    return "Info";
                case ELogMessageType::Warning:
                    return "Warning";
                case ELogMessageType::Error:
                    return "Error";
                default:
                    return "Unknown";
            }
        }
    }
}

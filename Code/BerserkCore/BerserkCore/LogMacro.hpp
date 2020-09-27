/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOGMACRO_HPP
#define BERSERK_LOGMACRO_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    namespace Details {
        void EngineLogAlways(const char *messageTag, const char *message);
        void EngineLogInfo(const char *messageTag, const char *message);
        void EngineLogWarning(const char *messageTag, const char *message);
        void EngineLogError(const char *messageTag, const char *message);
        void EngineLogFatalError(const char *messageTag, const char *message);
    }

#define BERSERK_LOG_ALWAYS(message) \
    Details::EngineLogAlways("default", message);

#define BERSERK_LOG_INFO(message) \
    Details::EngineLogInfo("default", message);

#define BERSERK_LOG_WARNING(message) \
    Details::EngineLogWarning("default", message);

#define BERSERK_LOG_ERROR(message) \
    Details::EngineLogError("default", message);

#define BERSERK_FATAL_ERROR(message) \
    Details::EngineLogFatalError("default", message);

}

#endif //BERSERK_LOGMACRO_HPP
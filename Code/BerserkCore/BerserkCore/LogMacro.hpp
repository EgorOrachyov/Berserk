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
        void engineLogAlways(const char *messageTag, const char *message);
        void engineLogInfo(const char *messageTag, const char *message);
        void engineLogWarning(const char *messageTag, const char *message);
        void engineLogError(const char *messageTag, const char *message);
        void engineLogFatalError(const char *messageTag, const char *message);
    }

#define BERSERK_LOG_ALWAYS(message) \
    Details::engineLogAlways("default", message);

#define BERSERK_LOG_INFO(message) \
    Details::engineLogInfo("default", message);

#define BERSERK_LOG_WARNING(message) \
    Details::engineLogWarning("default", message);

#define BERSERK_LOG_ERROR(message) \
    Details::engineLogError("default", message);

#define BERSERK_FATAL_ERROR(message) \
    Details::engineLogFatalError("default", message);

}

#endif //BERSERK_LOGMACRO_HPP
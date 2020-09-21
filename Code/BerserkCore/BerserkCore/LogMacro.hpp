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
        void engineOnInfo(const char *messageTag, const char *message);
        void engineOnWarning(const char *messageTag, const char *message);
        void engineOnError(const char *messageTag, const char *message);
        void engineOnFatalError(const char *messageTag, const char *message);
    }

#define BERSERK_INFO(message) \
    Details::engineOnInfo("default", message);

#define BERSERK_WARNING(message) \
    Details::engineOnWarning("default", message);

#define BERSERK_ERROR(message) \
    Details::engineOnError("default", message);

#define BERSERK_FATAL_ERROR(message) \
    Details::engineOnFatalError("default", message);

}

#endif //BERSERK_LOGMACRO_HPP
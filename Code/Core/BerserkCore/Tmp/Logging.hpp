/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOGGING_HPP
#define BERSERK_LOGGING_HPP

#include <BerserkCore/Log.hpp>
#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/String/StringBuilder.hpp>

namespace Berserk {

    class Logging {
    public:
        static void EngineLog(ELogMessageType messageType, const char* messageTag, const char* message, uint32 messageSize, bool abort);

        static void EngineLogTArgsImpl(StringBuilder& builder) {
            // Left untouched
        }

        template <typename T>
        static void EngineLogTArgsImpl(StringBuilder& builder, T&& arg) {
            builder << arg;
        }

        template <typename T, typename ... TArgs>
        static void EngineLogTArgsImpl(StringBuilder& builder, const T& arg, TArgs&& ... args) {
            builder << arg;
            EngineLogTArgsImpl(builder, std::forward<TArgs>(args)...);
        }

        template <typename ... TArgs>
        static void EngineLogTArgs(bool abort, ELogMessageType messageType, const char* messageTag, TArgs&& ... args) {
            StringBuilder builder;
            EngineLogTArgsImpl(builder, std::forward<TArgs>(args)...);
            auto& buffer = builder.GetBuffer();
            EngineLog(messageType, messageTag, buffer.GetData(), buffer.GetSize(), abort);
        }
    };

#define BERSERK_LOG_ALWAYS(...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Always, "Default", __VA_ARGS__);

#define BERSERK_LOG_INFO(...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Info, "Default", __VA_ARGS__);

#define BERSERK_LOG_WARNING(...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Warning, "Default", __VA_ARGS__);

#define BERSERK_LOG_ERROR(...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Error, "Default", __VA_ARGS__);

#define BERSERK_FATAL_ERROR(...) \
    Logging::EngineLogTArgs(true, ELogMessageType::Error, "Default", __VA_ARGS__);

#define BERSERK_LOG_CAT_ALWAYS(category, ...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Always, category, __VA_ARGS__);

#define BERSERK_LOG_CAT_INFO(category, ...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Info, category, __VA_ARGS__);

#define BERSERK_LOG_CAT_WARNING(category, ...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Warning, category, __VA_ARGS__);

#define BERSERK_LOG_CAT_ERROR(category, ...) \
    Logging::EngineLogTArgs(false, ELogMessageType::Error, category, __VA_ARGS__);

#define BERSERK_FATAL_CAT_ERROR(category, ...) \
    Logging::EngineLogTArgs(true, ELogMessageType::Error, category, __VA_ARGS__);

}

#endif //BERSERK_LOGGING_HPP
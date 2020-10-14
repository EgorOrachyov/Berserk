/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Assertion.hpp>
#include <BerserkCore/Application.hpp>
#include <BerserkCore/String/StringBuilder.hpp>

namespace Berserk{
    namespace Details {
        void AssertionOnFail(const char* what, uint64 line, const char* function, const char* file) {
            StringBuilder builder;
            const TArray<char> &buffer = builder.GetBuffer();

            builder << "ASSERTION FAILED: " << what
                    << " at [" << line << "," << function << "," << file << "]";

            auto& app = Application::Get();
            auto& log = app.GetLog();

            log.LogMessage(ELogMessageType::Error, "Assert", buffer.GetData(), buffer.GetSize());
            app.Abort(buffer.GetData(), buffer.GetSize());
        }

        void AssertionOnFail(const char* what, const char* msg, uint64 line, const char* function, const char* file) {
            StringBuilder builder;
            const TArray<char> &buffer = builder.GetBuffer();

            builder << "Assertion Failed: " << what << " Message: " << msg
                    << " at [" << line << "," << function << "," << file << "]";

            auto& app = Application::Get();
            auto& log = app.GetLog();

            log.LogMessage(ELogMessageType::Error, "Assert", buffer.GetData(), buffer.GetSize());
            app.Abort(buffer.GetData(), buffer.GetSize());
        }
    }
}

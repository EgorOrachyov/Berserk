/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_LOG_HPP
#define BERSERK_LOG_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class ELogMessageType {
        Info = 0,
        Warning,
        Error,
        Always
    };

    class LogListener {
    public:
        virtual ~LogListener() = default;
        virtual void OnLog(ELogMessageType messageType, const char* messageTag, const char* message) = 0;
    };

    class LogVisitor {
    public:
        virtual ~LogVisitor() = default;
        virtual void OnEntry(uint32 index, ELogMessageType messageType, const char* messageTag, const char* message) = 0;
    };

    class Log {
    public:
        virtual ~Log() = default;
        virtual void AddListener(LogListener& listener) = 0;
        virtual void RemoveListener(LogListener& listener) = 0;
        virtual void LogMessage(ELogMessageType messageType, const char* messageTag, const char* message) = 0;
        virtual void LogMessage(ELogMessageType messageType, const char* messageTag, const char* message, uint32 messageLength) = 0;
        virtual void DumpMessages(LogVisitor& observer) const = 0;
        static const char* GetLogMessageTypeAsStr(ELogMessageType messageType);
    };

}

#endif //BERSERK_LOG_HPP
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

#include <core/Engine.hpp>
#include <core/io/LoggerListenerOutput.hpp>

#include <sstream>

BRK_NS_BEGIN

void LoggerListenerOutput::SetName(String name) {
    mName = std::move(name);
}

void LoggerListenerOutput::SetLevel(Logger::Level level) {
    mLevel = level;
}

void LoggerListenerOutput::OnEntry(const Logger::Entry &entry) const {
    if (static_cast<uint32>(mLevel) >= static_cast<uint32>(entry.level)) {
        auto &output = Engine::Instance().GetOutput();
        auto &fs = Engine::Instance().GetFileSystem();

        std::stringstream stream;
        stream << "[" << LoggerLevelToStr(entry.level) << "] "
               << "[" << mName << "] "
               << "[" << fs.GetFileName(entry.file) << "] "
               << entry.message << std::endl;

        switch (entry.level) {
            case Logger::Level::Error:
                output.WriteError(stream.str());
                return;
            case Logger::Level::Warning:
                output.WriteWarning(stream.str());
                return;
            default:
                output.Write(stream.str());
                return;
        }
    }
}

BRK_NS_END
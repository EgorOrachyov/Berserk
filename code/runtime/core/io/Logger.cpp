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

#include <core/io/Logger.hpp>

#include <algorithm>
#include <cassert>

BRK_NS_BEGIN

void Logger::SetLevel(berserk::Logger::Level level) {
    mLevel.store(level);
}

void Logger::SetSize(size_t size) {
    assert(size > 0);
    std::lock_guard<std::mutex> guard(mMutex);
    mSize = size;
}

void Logger::SetActive(bool active) {
    mActive.store(active);
}

void Logger::AddListener(Logger::Listener listener) {
    std::lock_guard<std::mutex> guard(mMutex);
    mListeners.push_back(std::move(listener));
}

void Logger::Log(Logger::Level level, String message, String function, String file, size_t line) {
    Entry entry{std::move(message), std::move(function), std::move(file), line, level};
    AddEntry(std::move(entry));
}

void Logger::LogInfo(String message, String function, String file, size_t line) {
    Entry entry{std::move(message), std::move(function), std::move(file), line, Level::Info};
    AddEntry(std::move(entry));
}

void Logger::LogWarning(String message, String function, String file, size_t line) {
    Entry entry{std::move(message), std::move(function), std::move(file), line, Level::Warning};
    AddEntry(std::move(entry));
}

void Logger::LogError(String message, String function, String file, size_t line) {
    Entry entry{std::move(message), std::move(function), std::move(file), line, Level::Error};
    AddEntry(std::move(entry));
}

void Logger::Shrink() {
    while (mEntries.size() >= mSize)
        mEntries.pop();
}

void Logger::AddEntry(Logger::Entry &&entry) {
    std::lock_guard<std::mutex> guard(mMutex);

    if (ShouldLog(entry.level)) {
        // Remove old entries
        Shrink();

        // Notify listeners
        std::for_each(mListeners.begin(), mListeners.end(), [&](Listener &listener) { listener(entry); });

        // Store new entry
        mEntries.push(std::move(entry));
    }
}

Logger::Level Logger::GetLevel() const {
    return mLevel.load();
}

bool Logger::IsActive() const {
    return mActive.load();
}

bool Logger::ShouldLog(Level level) const {
    return IsActive() && static_cast<uint32>(GetLevel()) <= static_cast<uint32>(level);
}

Logger &Logger::Instance() {
    return gLogger;
}

Logger Logger::gLogger;

BRK_NS_END
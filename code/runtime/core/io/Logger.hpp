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

#ifndef BERSERK_LOGGER_HPP
#define BERSERK_LOGGER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <sstream>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Logger
 * @brief Simpler engine logger class to maintain text log
 */
class Logger final {
public:
    /** @brief Level of accepted messages */
    enum class Level {
        /** Useful for debugging and prototyping things */
        Info = 0,
        /** Code correct, by we need attract user attention */
        Warning = 1,
        /** Notify about error, invalid program state */
        Error = 2
    };

    /** @brief Log entry */
    struct Entry {
        String message;
        String function;
        String file;
        size_t line;
        Level level;
    };

    using Listener = std::function<void(const Entry &)>;
    static const size_t DEFAULT_SIZE = 100;

    BRK_API Logger() = default;

    BRK_API void SetLevel(Level level);
    BRK_API void SetSize(size_t size = DEFAULT_SIZE);
    BRK_API void SetActive(bool active = true);
    BRK_API void AddListener(Listener listener);

    BRK_API void Log(Level level, String message, String function = "", String file = "", size_t line = 0);
    BRK_API void LogInfo(String message, String function = "", String file = "", size_t line = 0);
    BRK_API void LogWarning(String message, String function = "", String file = "", size_t line = 0);
    BRK_API void LogError(String message, String function = "", String file = "", size_t line = 0);

    BRK_API Level GetLevel() const;
    BRK_API bool IsActive() const;
    BRK_API bool ShouldLog(Level level) const;

    BRK_API static Logger &Instance();

private:
    void Shrink();
    void AddEntry(Entry &&entry);

private:
    std::queue<Entry> mEntries;
    std::vector<Listener> mListeners;
    std::atomic<Level> mLevel{Level::Info};
    std::atomic_bool mActive{true};
    size_t mSize = DEFAULT_SIZE;

    mutable std::mutex mMutex;
    static Logger gLogger;
};

namespace {
    /** @return Logger level to str */
    inline const char *LoggerLevelToStr(Logger::Level level) {
        switch (level) {
            case Logger::Level::Info:
                return "Info";
            case Logger::Level::Warning:
                return "Warning";
            case Logger::Level::Error:
                return "Error";
            default:
                return "Unknown";
        }
    }
}// namespace

#define BRK_LOG(log, level, message)                                                               \
    do {                                                                                           \
        (log).Log(level, message, __FUNCTION__, __FILE__, static_cast<BRK_NS ::size_t>(__LINE__)); \
    } while (false);

#define BRK_LOG_MESSAGE(log, level, message) \
    if ((log).ShouldLog(level)) {            \
        std::stringstream ss;                \
        ss << message;                       \
        BRK_LOG(log, level, ss.str());       \
    } else {                                 \
    }

/** Log info message to the default logger */
#define BRK_INFO(message) \
    BRK_LOG_MESSAGE(BRK_NS ::Logger::Instance(), BRK_NS ::Logger::Level::Info, message)

/** Log warning message to the default logger */
#define BRK_WARNING(message) \
    BRK_LOG_MESSAGE(BRK_NS ::Logger::Instance(), BRK_NS ::Logger::Level::Warning, message)

/** Log error message to the default logger */
#define BRK_ERROR(message) \
    BRK_LOG_MESSAGE(BRK_NS ::Logger::Instance(), BRK_NS ::Logger::Level::Error, message)

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_LOGGER_HPP

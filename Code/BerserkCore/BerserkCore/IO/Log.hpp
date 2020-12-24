/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LOG_HPP
#define BERSERK_LOG_HPP

#include <BerserkCore/Misc/DateTime.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Containers/Array.hpp>
#include <BerserkCore/Threading/Synchronization.hpp>

namespace Berserk {

    /**
     * @brief Logger
     *
     * Log for logging messages. Allows to log messages with different categories and verbosity.
     * Provides utility to store message in text or html form.
     * Allows to observe logger messages to be notified when new entry is added.
     */
    class Log {
    public:

        enum class Verbosity {
            Fatal = 0,
            Error,
            Warning,
            Info,
            Log
        };

        class Entry {
        public:

            explicit Entry(String&& category, String&& message, Verbosity verbosity, TimeStamp timeStamp)
                : category(std::move(category)), message(std::move(message)), verbosity(verbosity), timeStamp(timeStamp) {

            }

            String category; // todo: hashed string name
            String message;
            Verbosity verbosity = Verbosity::Log;
            TimeStamp timeStamp;
        };


    private:
        Array<Entry> mEntries;
        mutable RecursiveMutex mMutex;
    };

}

#endif //BERSERK_LOG_HPP
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/IO/Log.hpp>
#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    Log::Entry::Entry(String &&category, String &&message, Verbosity verbosity)
        : category(std::move(category)), message(std::move(message)), verbosity(verbosity), timeStamp(Platform::System::GetTimeStamp()) {

    }

    void Log::LogMessage(String &&category, String &&message, Verbosity verbosity) {
        Guard<RecursiveMutex> guard(mMutex);
        auto& entry = mEntries.Emplace(std::move(category), std::move(message), verbosity);

        auto date = Date();
        auto time = Time();
        Platform::System::GetDateTime(entry.timeStamp, date, time, TimeType::Local);

        auto formatted = mFormatter.Print(BERSERK_TEXT("[{0}.{1}.{2} {3}:{4}:{5}][{6}] {7}:{8}\n"),
                                          date.year, date.GetMonthNumber(), date.dayMonth,
                                          time.hour, time.min, time.sec,
                                          entry.verbosity, entry.category, entry.message);

        if (verbosity == Verbosity::Error || verbosity == Verbosity::Fatal) {
            auto& err = Platform::System::Error();
            err.Write(formatted);
        }
        else if (verbosity == Verbosity::Warning || verbosity == Verbosity::Info) {
            auto& out = Platform::System::Out();
            out.Write(formatted);
        }
    }

    void Log::GetEntries(Array<Entry> &entries) const {
        Guard<RecursiveMutex> guard(mMutex);
        entries = mEntries;
    }

}

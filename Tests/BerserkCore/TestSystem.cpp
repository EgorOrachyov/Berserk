/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Misc/Crc32.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/Formatter.hpp>
#include <BerserkCore/Platform/System.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(SystemFixture)

template <typename Stream>
Stream& operator << (Stream& stream, const String& string) {
    return stream << string.GetStr();
}

TEST_F(SystemFixture, TimeStamp) {
    Formatter<> format;

    auto localTime = Platform::System::GetTime(TimeType::Local);
    auto globalTime = Platform::System::GetTime(TimeType::Global);

    auto timeInfo = format.Print(BERSERK_TEXT("Local/Global time: {0}:{1}:{2} vs {3}:{4}:{5}"),
                                 localTime.hour, localTime.min, localTime.sec,
                                 globalTime.hour, globalTime.min, globalTime.sec);

    std::cout << timeInfo << std::endl;
}

TEST_F(SystemFixture, DateStamp) {
    Formatter<> format;

    auto localDate = Platform::System::GetDate(TimeType::Local);
    auto globalDate = Platform::System::GetDate(TimeType::Global);

    auto dateInfo = format.Print(BERSERK_TEXT("Local/Global date: {0} {1} {2} vs {3} {4} {5}"),
                                 localDate.weekday, localDate.month, localDate.year,
                                 globalDate.weekday, globalDate.month, globalDate.year);

    std::cout << dateInfo << std::endl;
}

TEST_F(SystemFixture, DateTime) {
    Formatter<> format;

    auto timestamp = Platform::System::GetTimeStamp();
    auto date = Date();
    auto time = Time();

    Platform::System::GetDateTime(timestamp, date, time, TimeType::Local);

    std::cout << format.Print(BERSERK_TEXT("{0}.{1}.{2} {3}:{4}:{5}"),
                              date.year, date.GetMonthNumber(), date.dayMonth,
                              time.hour, time.min, time.sec) << std::endl;

    Platform::System::GetDateTime(timestamp, date, time, TimeType::Global);

    std::cout << format.Print(BERSERK_TEXT("{0}.{1}.{2} {3}:{4}:{5}"),
                              date.year, date.GetMonthNumber(), date.dayMonth,
                              time.hour, time.min, time.sec) << std::endl;
}

TEST_F(SystemFixture, ConsoleOut) {
    auto& console = Platform::System::Out();

    console.Write(-0.0312f);
    console.WriteLine();

    console.Write(1230.0312);
    console.WriteLine();

    console.Write(-121423);
    console.WriteLine();

    console.Write(-121423235423);
    console.WriteLine();

    console.Write(151551423u);
    console.WriteLine();

    console.Write(1515514123423);
    console.WriteLine();

    console.Write(BERSERK_TEXT("Some fancy text zß水🍌"));
    console.WriteLine();

    console.Write(String(BERSERK_TEXT("Some fancy text zß水🍌")));
    console.WriteLine();

    console.WriteF(BERSERK_TEXT("This is a {0} important in {1} message about {2} C"), BERSERK_TEXT("very"), 1231, -0.324f);
    console.WriteLine();

    console.Flush();
}

TEST_F(SystemFixture, ConsoleError) {
    auto& console = Platform::System::Error();

    console.Write(-0.0312f);
    console.WriteLine();

    console.Write(1230.0312);
    console.WriteLine();

    console.Write(-121423);
    console.WriteLine();

    console.Write(-121423235423);
    console.WriteLine();

    console.Write(151551423u);
    console.WriteLine();

    console.Write(1515514123423);
    console.WriteLine();

    console.Write(BERSERK_TEXT("Some fancy text zß水🍌"));
    console.WriteLine();

    console.Write(String(BERSERK_TEXT("Some fancy text zß水🍌")));
    console.WriteLine();

    console.WriteF(BERSERK_TEXT("This is a {0} important in {1} message about {2} C"), BERSERK_TEXT("very"), 1231, -0.324f);
    console.WriteLine();

    console.Flush();
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
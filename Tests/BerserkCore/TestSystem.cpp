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
#include <BerserkCore/Misc/Event.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/Formatter.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Debug/Debug.hpp>

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

TEST_F(SystemFixture, SystemLogger) {
    auto& log = Platform::System::Logger();

    log.LogMessage(BERSERK_TEXT("Dft"), BERSERK_TEXT("Some fancy message"), Log::Verbosity::Logging);
    log.LogMessage(BERSERK_TEXT("Dft"), BERSERK_TEXT("Some fancy message"), Log::Verbosity::Info);
    log.LogMessage(BERSERK_TEXT("Dft"), BERSERK_TEXT("Some fancy message"), Log::Verbosity::Warning);
    log.LogMessage(BERSERK_TEXT("Dft"), BERSERK_TEXT("Some fancy message"), Log::Verbosity::Error);
    log.LogMessage(BERSERK_TEXT("Dft"), BERSERK_TEXT("Some fancy message"), Log::Verbosity::Fatal);

    log.LogMessageF(BERSERK_TEXT("Ant"), Log::Verbosity::Logging, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Ant"), Log::Verbosity::Info, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Ant"), Log::Verbosity::Warning, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Ant"), Log::Verbosity::Error, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Ant"), Log::Verbosity::Fatal, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));

    log.LogMessageF(BERSERK_TEXT("Btw"), Log::Verbosity::Logging, BERSERK_TEXT("Another fancy format {0}"), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Btw"), Log::Verbosity::Info, BERSERK_TEXT("Another fancy format {0}"), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Btw"), Log::Verbosity::Warning, BERSERK_TEXT("Another fancy format {0}"), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Btw"), Log::Verbosity::Error, BERSERK_TEXT("Another fancy format {0}"), BERSERK_TEXT("zß水🍌"));
    log.LogMessageF(BERSERK_TEXT("Btw"), Log::Verbosity::Fatal, BERSERK_TEXT("Another fancy format {0}"), BERSERK_TEXT("zß水🍌"));
}

TEST_F(SystemFixture, SystemLoggerTextDump) {
    auto path = BERSERK_TEXT("LogSaved.txt");
    auto& log = Platform::System::Logger();

    auto messagesToGenerate = 100;

    for (auto i = 0; i < messagesToGenerate; i++) {
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Logging, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Info, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Warning, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Error, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Fatal, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    }

    log.SaveLog(path, Log::SaveFormat::Text);
}

TEST_F(SystemFixture, SystemLoggerHtmlDump) {
    auto path = BERSERK_TEXT("LogSaved.html");
    auto& log = Platform::System::Logger();

    auto messagesToGenerate = 100;

    for (auto i = 0; i < messagesToGenerate; i++) {
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Logging, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Info, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Warning, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Error, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
        log.LogMessageF(BERSERK_TEXT("Generated"), Log::Verbosity::Fatal, String(BERSERK_TEXT("Fancy format {0}")), BERSERK_TEXT("zß水🍌"));
    }

    log.SaveLog(path, Log::SaveFormat::Html);
}

TEST_F(SystemFixture, EventBase) {
    EventPublisher<const String&, uint64> event;
    Formatter<> formatter;

    auto action1 = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("First {0} {1}\n"), s, i + 1);
    };

    auto action2 = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Second {0} {1}\n"), s, i + 2);
    };

    auto hnd1 = event.Subscribe(action1);
    auto hnd2 = event.Subscribe(action2);

    event.Dispatch(BERSERK_TEXT("Hello"), 0x0);
    event.Dispatch(BERSERK_TEXT("Hello"), 0xc);
}

TEST_F(SystemFixture, EventDispatch) {
    EventPublisher<const String&, uint64> event;
    Formatter<> formatter;

    EventHnd hnd;

    auto test = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Test {0} {1}\n"), s, i + 0);
    };

    auto action = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Action {0} {1}\n"), s, i + 2);

        if (i == 0x0) {
            hnd = event.Subscribe(test);
        }

        if (i == 0xf) {
            hnd.Disconnect();
        }
    };

    auto a = event.Subscribe(action);

    event.Dispatch(BERSERK_TEXT("Hello"), 0x0);
    event.Dispatch(BERSERK_TEXT("Hello"), 0xc);
    event.Dispatch(BERSERK_TEXT("World"), 0xf);
}

TEST_F(SystemFixture, EventSubscribe) {
    EventPublisher<const String&, uint64> event;
    Formatter<> formatter;

    EventHnd hnd;

    auto test = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Test {0} {1}\n"), s, i + 0);
    };

    auto action = [&](const String& s, uint64 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Action {0} {1}\n"), s, i + 0);
        hnd = event.Subscribe(test);
    };

    auto a = event.Subscribe(action);

    event.Dispatch(BERSERK_TEXT("Hello"), 0x0);
    event.Dispatch(BERSERK_TEXT("World"), 0xf);
    event.Dispatch(BERSERK_TEXT("World"), 0xc);
}

TEST_F(SystemFixture, EventPendings) {
    EventPublisher<uint32> event;
    Formatter<> formatter;

    Array<EventHnd> handlers;
    Array<EventHnd> todisconnect;

    auto action = [&](uint32 i) {
        auto& out = Platform::System::Out();
        out.WriteFF(formatter, BERSERK_TEXT("Action {0}\n"), i);
        out.Flush();

        if (i == 1) {
            for (auto h: todisconnect) {
                h.Disconnect();
            }
        }
    };

    uint32 count = 5;

    for (auto i = 0; i < count; i++) {
        auto hnd = event.Subscribe(action);
        handlers.Add(hnd);
        todisconnect.Add(hnd);
    }

    event.Dispatch(0);
    event.Dispatch(1);
    event.Dispatch(2);
}

TEST_F(SystemFixture, EventShare) {
    auto act1 = [](uint32 i){
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Act[{1}]={0}"), i, 1);
    };

    auto act2 = [](uint32 i){
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Act[{1}]={0}"), i, 2);
    };

    auto act3 = [](uint32 i){
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Act[{1}]={0}"), i, 3);
    };

    EventPublisher<uint32> event1;

    // Get hnd to registered action
    auto hnd1 = event1.Subscribe(act1);
    auto hnd2 = event1.Subscribe(act2);
    auto hnd3 = event1.Subscribe(act3);

    // Dispatch event
    event1.Dispatch(1);

    // Share state
    EventPublisher<uint32> event2 = event1;
    event2.Dispatch(2);

    // Disconnect some handler
    hnd1.Disconnect();
    event1.Dispatch(3);

    // Clear all connections
    event2.Clear();
    event2.Dispatch(4);

    // Append log
    BERSERK_CORE_LOG_WARNING(BERSERK_TEXT("Fancy message zß水🍌 :)"));
    BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Fancy message zß水🍌 as error"));
    BERSERK_CORE_LOG_FATAL(BERSERK_TEXT("Fancy message zß水🍌 as fatal error"));

    // Save log as an example
    Platform::System::Logger().SaveLog(BERSERK_TEXT("EventShare.html"), Log::SaveFormat::Html);
}

TEST_F(SystemFixture, EventRecursive) {
    EventPublisher<uint32> event;

    auto action = [&](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Level={0}"), i);

        if (i > 0) {
            event.Dispatch(i - 1);
        }
    };

    auto hnd = event.Subscribe(action);

    event.Dispatch(5);
}

TEST_F(SystemFixture, EventRecursiveDisconnect) {
    EventPublisher<uint32> event;
    EventHnd hnd;

    auto action = [&](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Level={0}"), i);

        if (i == 4) {
            hnd.Disconnect();
        }

        if (i > 0) {
            event.Dispatch(i - 1);
        }
    };

    hnd = event.Subscribe(action);

    event.Dispatch(10);
}

TEST_F(SystemFixture, EventRecursiveConnect) {
    EventPublisher<uint32> event;
    EventHnd hnd;
    EventHnd hndRecursive;

    auto actionRecursive = [](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Recursive Level={0}"), i);
    };

    auto action = [&](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Level={0}"), i);

        if (i == 4) {
            hndRecursive = event.Subscribe(actionRecursive);
        }

        if (i > 0) {
            event.Dispatch(i - 1);
        }
    };

    hnd = event.Subscribe(action);

    event.Dispatch(6);
    event.Dispatch(3);
}

TEST_F(SystemFixture, EventRecursiveConnectDisconnect) {
    EventPublisher<uint32> event;
    EventHnd hnd;
    EventHnd hndRecursive;

    auto actionRecursive = [](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Recursive Level={0}"), i);
    };

    auto action = [&](uint32 i) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Level={0}"), i);

        if (i == 4) {
            hndRecursive = event.Subscribe(actionRecursive);
        }

        if (i == 1) {
            hnd.Disconnect();
            hndRecursive.Disconnect();
        }

        if (i > 0) {
            event.Dispatch(i - 1);
        }
    };

    hnd = event.Subscribe(action);

    event.Dispatch(6);
    event.Dispatch(3);
}

TEST_F(SystemFixture, EventConnectionsPool) {
    EventPublisher<> event;

    uint32 iterations = 10000;
    uint32 counter;

    Array<EventHnd> connections;

    for (auto i = 0; i < iterations; i++) {
        connections.Add(event.Subscribe([&](){
            counter += 1;
        }));
    }

    counter = 0;
    event.Dispatch();
    EXPECT_EQ(iterations, counter);

    for (auto i = 0; i < iterations; i++) {
        EXPECT_TRUE(connections[i].IsConnected());
    }

    connections.Clear();

    counter = 0;
    event.Dispatch();
    EXPECT_EQ(0, counter);
}

BERSERK_GTEST_MAIN
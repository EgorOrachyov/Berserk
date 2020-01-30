/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Mutex.h>
#include <Platform/System.h>
#include <Platform/Input.h>
#include <IO/Logs.h>

#include <ErrorMacro.h>
#include <TestMacro.h>

#include <thread>

using namespace Berserk;

BERSERK_TEST_SECTION(Platform)
{
    BERSERK_TEST(Input)
    {
        EModifierMask mods[] = {
            EModifierMask::Shift,
            EModifierMask::Alt,
            EModifierMask::Control,
            EModifierMask::CapsLock,
            EModifierMask::NumLock
        };

        for (auto mod: mods) {
            printf("%u \n", (uint32) mod);
        }
    };

    BERSERK_TEST(ErrorMacro)
    {
        BERSERK_ERROR("Error without return of abort")
        BERSERK_ERROR("Error without return of abort")
        BERSERK_WARNING("Waring")
        BERSERK_WARNING("Some another waring")

        auto f = [](const char* message, uint64 line, const char* function, const char* file, EErrorType errorType) {
            printf("Line: %llu Function: %s File: %s\n", line, function, file);
            printf("Message: '%s' EErrorType: '%s'\n", message, ErrorMacro::getErrorType(errorType));
        };

        ErrorMacro::forEachError(f);
    };

    BERSERK_TEST(Atomic)
    {
        auto atomic = ISystem::getSingleton().createAtomic();
        auto job = [&](){ while (atomic->load() % 2 == 0) atomic->add(2); };

        std::thread thread(job);
        std::this_thread::yield();

        while (atomic->load() < 1000);
        atomic->add(1);
        thread.join();

        printf("%i\n", atomic->load());
    };

    BERSERK_TEST(File)
    {
        auto input = ISystem::getSingleton().openFile("TestRead.txt", EFileMode::Read);

        if (input->isOpen()) {
            char buffer[100];
            auto size = input->getSize();
            input->read(buffer, size);
            buffer[size] = '\0';
            printf("File: %s\n", buffer);
        }

        auto output = ISystem::getSingleton().openFile("TestWrite.txt", EFileMode::Write);

        if (output->isOpen()) {
            CString message = "Hello! This will be written to the file";
            output->write(message.data(), message.length());
        }
    };

    BERSERK_TEST(WindowSystem)
    {
        ISystem::VideoMode mode{};
        ISystem& Sys = ISystem::getSingleton();

        Sys.initialize("Test window", mode);

        while (!Sys.shouldClose(ISystem::MAIN_WINDOW)) Sys.update();

        auto size = Sys.getWindowSize(ISystem::MAIN_WINDOW);
        printf("Window size: %i %i\n", size[0], size[1]);

        auto time = Sys.getTime();
        printf("Now %i:%i:%i\n", time.hour, time.min, time.sec);

        auto& out = Sys.getOutput();
        out.print("Sync output device\n");
        out.print("Some messages\n");

        Sys.finalize();
    };

    BERSERK_TEST(LogFile)
    {
        TUnq<IFile> file = ISystem::getSingleton().openFile("LogFile.txt", EFileMode::Write);
        LogFile log(file);

        log.logf(ELogVerbosity::Info, "First log message");
        log.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");

        auto& sysLog = ISystem::getSingleton().getLog();
        sysLog.logf(ELogVerbosity::Info, "First log message");
        sysLog.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");
    };
}

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/IDirectory.h>
#include <Platform/Mutex.h>
#include <Platform/ISystem.h>
#include <Platform/IInput.h>
#include <IO/Logs.h>

#include <TimeValue.h>
#include <ErrorMacro.h>
#include <TestMacro.h>
#include <String/WString.h>

#include <thread>
#include <memory>

using namespace Berserk;

BERSERK_TEST_SECTION(Platform)
{
    BERSERK_TEST(IDirectory)
    {
        TPtrUnique<IDirectory> dir = ISystem::getSingleton().openDirectory(".");

        printf("Is open: %i\n", dir->isOpen());
        printf("Path: %s\n", dir->getPath().data());
        printf("Real path: %s\n", dir->getRealPath().data());

        if (dir->isOpen()) {
            const auto& entries = dir->getEntries();

            for (auto& e: entries) {
                printf("Entry name: [%s] len: [%u] type [%s]\n", e.name.data(), e.name.length(), IDirectory::typeToString(e.type));
            }
        }
    };

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
        struct Listener : public IInputListenerKeyboard,
                          public IInputListenerMouse,
                          public IInputListenerJoystick {

            // IInputListenerKeyboard
            bool onKeyboardEvent(const InputEventKeyboard &event) override {
                if (event.inputAction == EInputAction::Text) {
                    // receive some os text input in UTF-32 format
                    wprintf(L"%lc", event.codepoint);
                }

                return false;
            }
            // IInputListenerMouse
            bool onMouseEvent(const InputEventMouse &event) override {
                if (event.inputAction == EInputAction::Move) {
                    printf("Mouse pos: %i %i, delta: %i %i\n",
                            event.position.x(), event.position.y(),
                            event.delta.x(), event.delta.y());
                }

                return false;
            }
            // IInputListenerJoystick
            bool onJoystickEvent(const InputEventJoystick &event) override {
                if (event.inputAction == EInputAction::Move && event.value > 0.4f) {
                    printf("Axis: %i value: %f \n", event.axis, event.value);
                }
                if (event.inputAction == EInputAction::Press) {
                    printf("Press Button: %i\n", event.button);
                }
                if (event.inputAction == EInputAction::Release) {
                    printf("Release Button: %i\n", event.button);
                }

                return false;
            }
        } listener;

        ISystem::VideoMode mode{};
        mode.resizeable = true;

        auto& Sys = ISystem::getSingleton();
        auto Win = ISystem::MAIN_WINDOW;

        Sys.vinitialize("Test window", mode, ERenderDeviceType::OpenGL);
        //IInput::getSingleton().addMouseListener(listener);
        IInput::getSingleton().addKeyboardListener(listener);
        IInput::getSingleton().addJoystickListener(listener);


        TimeValue prev = TimeValue::now();
        TimeValue dur = TimeValue().setMilliseconds(40.0f);

        while (!Sys.shouldClose(ISystem::MAIN_WINDOW)) {
            auto until = dur + prev;
            auto cur = TimeValue::wait(until);
            auto eps = cur - prev; prev = cur;

            //printf("Time: %lf\n", eps.getMilliseconds());

            Sys.vupdate();

            if (Sys.isResized(Win)) {
                auto s = Sys.getWindowSize(Win);
                printf("New size: %i %i\n", s[0], s[1]);
            }

            if (Sys.isMoved(Win)) {
                auto p = Sys.getWindowPos(Win);
                printf("New pos: %i %i\n", p[0], p[1]);
            }

            if (Sys.isMinimized(Win))
                printf("Minimized: %s\n", Sys.getWindowCaption(Win).data());

            if (Sys.isRestored(Win))
                printf("Restored: %s\n", Sys.getWindowCaption(Win).data());

            if (IInput::getSingleton().hasDropInput()) {
                TArray<CString> drop;
                IInput::getSingleton().getDropInput(drop);

                for (auto& s: drop) {
                    auto w = WString::from(s);
                    wprintf(L"Dropped: '%ls' [%u]\n", w.data(), w.length());
                }
            }
        }

        //IInput::getSingleton().removeMouseListener(listener);
        IInput::getSingleton().removeKeyboardListener(listener);
        IInput::getSingleton().removeJoystickListener(listener);

        auto size = Sys.getWindowSize(ISystem::MAIN_WINDOW);
        printf("Window size: %i %i\n", size[0], size[1]);

        auto time = Sys.getTime();
        printf("Now %i:%i:%i\n", time.hour, time.min, time.sec);

        auto& out = Sys.getOutputDevice();
        out.print("Sync output device\n");
        out.print("Some messages\n");

        Sys.vfinalize();
    };

    BERSERK_TEST(LogFile)
    {
        TPtrUnique<IFile> file = ISystem::getSingleton().openFile("LogFile.txt", EFileMode::Write);
        LogFile log(file);

        log.logf(ELogVerbosity::Info, "First log message");
        log.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");

        auto& sysLog = ISystem::getSingleton().getLog();
        sysLog.logf(ELogVerbosity::Info, "First log message");
        sysLog.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");
    };

    BERSERK_TEST(TimeValue)
    {
        TimeValue v = TimeValue::nowAsTime();
        ISystem::Time t;

        printf("%fms\n", TimeValue::now().getMilliseconds());

        t = ISystem::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        t = ISystem::getSingleton().getTime();
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        printf("%fms\n", TimeValue::now().getMilliseconds());
    };

    BERSERK_TEST(Time)
    {
        TimeValue v = TimeValue::nowAsTime();
        ISystem::Time t;

        t = ISystem::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        t.dayWeek = 0;
        t.dayYear = 0;
        t.sec += 1;
        v = ISystem::getSingleton().getTimeValue(t);

        t = ISystem::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);
    };
}

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Directory.h>
#include <Platform/Mutex.h>
#include <Platform/System.h>
#include <Platform/Input.h>
#include <Platform/WindowManager.h>
#include <Platform/Thread.h>
#include <Platform/Async.h>
#include <Platform/TSync.h>
#include <IO/Logs.h>

#include <Math/TRange.h>
#include <TimeValue.h>
#include <ErrorMacro.h>
#include <TestMacro.h>
#include <String/WString.h>
#include <String/CString.h>

#include <thread>
#include <memory>

using namespace Berserk;

BERSERK_TEST_SECTION(Platform)
{
    BERSERK_TEST(IDirectory)
    {
        TPtrUnique<Directory> dir = System::getSingleton().openDirectory(".");

        printf("Is open: %i\n", dir->isOpen());
        printf("Path: %s\n", dir->getPath().data());
        printf("Real path: %s\n", dir->getRealPath().data());

        if (dir->isOpen()) {
            const auto& entries = dir->getEntries();

            for (auto& e: entries) {
                printf("Entry name: [%s] len: [%u] type [%s]\n", e.name.data(), e.name.length(), Directory::typeToString(e.type));
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

    BERSERK_TEST_COND(ErrorMacro, false)
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

    BERSERK_TEST_COND(File, false)
    {
        auto input = System::getSingleton().openFile("TestRead.txt", EFileMode::Read);

        if (input->isOpen()) {
            char buffer[100];
            auto size = input->getSize();
            input->read(buffer, size);
            buffer[size] = '\0';
            printf("File: %s\n", buffer);
        }

        auto output = System::getSingleton().openFile("TestWrite.txt", EFileMode::Write);

        if (output->isOpen()) {
            CString message = "Hello! This will be written to the file";
            output->write(message.data(), message.length());
        }
    };

    BERSERK_TEST_COND(WindowSystem, true)
    {
        struct Listener : public InputListenerKeyboard,
                          public InputListenerMouse,
                          public InputListenerJoystick {

            // InputListenerKeyboard
            bool onKeyboardEvent(const InputEventKeyboard &event) override {
                if (event.inputAction == EInputAction::Text) {
                    // receive some os text input in UTF-32 format
                    wprintf(L"%lc", event.codepoint);
                }

                return false;
            }
            // InputListenerMouse
            bool onMouseEvent(const InputEventMouse &event) override {
                if (event.inputAction == EInputAction::Move) {
                    printf("Mouse pos: %i %i, delta: %i %i\n",
                            event.position.x(), event.position.y(),
                            event.delta.x(), event.delta.y());
                }

                return false;
            }
            // InputListenerJoystick
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

        class WindowListener : public WindowResizeListener,
                               public WindowPositionListener,
                               public WindowStateListener {
        public:
            void onResized(Size2i oldSize, Size2i newSize) override {
                printf("Size: %i %i -> %i %i\n", oldSize[0], oldSize[1], newSize[0], newSize[1]);
            }

            void onMoved(Point2i oldPosition, Point2i newPosition) override {
                printf("Pos: %i %i -> %i %i\n", oldPosition[0], oldPosition[1], newPosition[0], newPosition[1]);
            }

            void onStateChanged(EWindowState oldState, EWindowState newState) override {
                printf("State: %s -> %s\n", getState(oldState), getState(newState));
            }

            const char* getState(EWindowState state) const {
                switch (state) {
                    case EWindowState::Normal:
                        return "Normal";
                    case EWindowState::Minimised:
                        return "Minimised";
                    case EWindowState::Closed:
                        return "Closed";
                    default:
                        return "";
                }
            }
        } windowListener;

        auto& Sys = System::getSingleton();
        auto& WindowMan = WindowManager::getSingleton();

        Sys.initialize("Window", "Test window", {640, 480}, false, ERenderDeviceType::OpenGL, CString(), false);
        auto window = WindowMan.find("Window");
        window->setLimits({500,400},{1920,1280});
        window->addPositionListener(windowListener);
        window->addResizeListener(windowListener);
        window->addStateListener(windowListener);

        //Input::getSingleton().addMouseListener(listener);
        Input::getSingleton().addKeyboardListener(listener);
        Input::getSingleton().addJoystickListener(listener);

        TimeValue prev = TimeValue::now();
        TimeValue dur = TimeValue().setMilliseconds(40.0f);

        while (!window->shouldClose() && !window->isClosed()) {
            auto until = dur + prev;
            auto cur = TimeValue::wait(until);
            auto eps = cur - prev; prev = cur;

            //printf("Time: %lf\n", eps.getMilliseconds());

            Sys.update();

            if (Input::getSingleton().hasDropInput()) {
                TArray<CString> drop;
                Input::getSingleton().getDropInput(drop);

                for (auto& s: drop) {
                    auto w = WString::from(s);
                    wprintf(L"Dropped: '%ls' [%u]\n", w.data(), w.length());
                }
            }
        }

        //Input::getSingleton().removeMouseListener(listener);
        Input::getSingleton().removeKeyboardListener(listener);
        Input::getSingleton().removeJoystickListener(listener);

        auto size = window->getSize();
        printf("Window size: %i %i\n", size[0], size[1]);

        auto time = Sys.getTime();
        printf("Now %i:%i:%i\n", time.hour, time.min, time.sec);

        auto path = Sys.getExecutablePath();
        printf("Executable path: %s\n", path.data());

        auto enginePath = Sys.getRootPath();
        printf("Engine path: %s\n", enginePath.data());

        auto cachePath = Sys.getCachePath();
        printf("Cache path: %s\n", cachePath.data());

        auto configPath = Sys.getConfigPath();
        printf("Config path: %s\n", configPath.data());

        auto& out = Sys.getOutputDevice();
        out.print("Sync output device\n");
        out.print("Some messages\n");

        Sys.finalize();
    };

    BERSERK_TEST_COND(LogFile, false)
    {
        TPtrUnique<File> file = System::getSingleton().openFile("LogFile.txt", EFileMode::Write);
        LogFile log(file);

        log.logf(ELogVerbosity::Info, "First log message");
        log.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");

        auto& sysLog = System::getSingleton().getLog();
        sysLog.logf(ELogVerbosity::Info, "First log message");
        sysLog.logf(ELogVerbosity::Error, "First log error: %s", "some useful info");
    };

    BERSERK_TEST_COND(TimeValue, false)
    {
        TimeValue v = TimeValue::nowAsTime();
        System::Time t;

        printf("%fms\n", TimeValue::now().getMilliseconds());

        t = System::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        t = System::getSingleton().getTime();
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        printf("%fms\n", TimeValue::now().getMilliseconds());
    };

    BERSERK_TEST_COND(Time, false)
    {
        TimeValue v = TimeValue::nowAsTime();
        System::Time t;

        t = System::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);

        t.dayWeek = 0;
        t.dayYear = 0;
        t.sec += 1;
        v = System::getSingleton().getTimeValue(t);

        t = System::getSingleton().getTime(v);
        printf("%i.%i.%i %i:%i:%i\n", t.year, t.month + 1, t.dayMonth + 1, t.hour, t.min, t.sec);
    };

    BERSERK_TEST_COND(Thread, true)
    {
        Thread::setNameCurrent("Main");

        auto function = [](){
            for (uint32 i = 0; i < 1000; i++) {
                static CString n = Thread::getNameCurrent();
                printf("Thread: %s\n", n.data());
                Thread::sleepCurrent(1000);
            }
        };

        auto t = Thread::create(function);

        t->setName("Job");

        for (uint32 i = 0; i < 1000; i++) {
            static CString n = Thread::getNameCurrent();
            printf("Thread: %s\n", n.data());
            Thread::sleepCurrent(1000 * 10);
        }

        t->tryJoin();
    };

    BERSERK_TEST_COND(Library, false)
    {
        auto& system = System::getSingleton();
        auto library = system.openLibrary("some.so");
    };
}

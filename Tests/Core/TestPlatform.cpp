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

#include <Errors.h>
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
            printf("Message: '%s' EErrorType: '%s'\n", message, Errors::getErrorType(errorType));
        };

        Errors::forEachError(f);
    };

    BERSERK_TEST(Atomic)
    {
        auto atomic = System::getSingleton().createAtomic();
        auto job = [&](){ while (atomic->load() % 2 == 0) atomic->add(2); };

        std::thread thread(job);
        std::this_thread::yield();

        while (atomic->load() < 1000);
        atomic->add(1);
        thread.join();

        printf("%i\n", atomic->load());
    };
}

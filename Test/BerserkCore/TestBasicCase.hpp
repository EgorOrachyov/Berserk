/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TESTBASICCASE_HPP
#define BERSERK_TESTBASICCASE_HPP

#include <gtest/gtest.h>
#include <BerserkCore/BuildOptions.hpp>
#include <BerserkCore/Application.hpp>
#include <BerserkPlatform/PlatformLog.hpp>

#ifdef BERSERK_TARGET_LINUX
#include <BerserkPlatform/Linux.hpp>
#endif

using namespace Berserk;

class App: public Application {
public:
    App() : Application() { }

    Allocator *GetGlobalAllocator() override {
        return &alloc;
    }

    Log *GetLog() override {
        return &platformLog;
    }

    System *GetSystem() override {
        return &system;
    }

private:

    struct Alloc: public Allocator {
    public:
        ~Alloc() override { printf("Alloc stat: %i\n", allocStat);  };
        void *Allocate(uint64 size) override { allocStat += 1; return ::malloc(size); }
        void Free(void *memory) override { ::free(memory); }
        int32 allocStat = 0;
    };

    Alloc alloc;
    PlatformLog platformLog;

#ifdef BERSERK_TARGET_LINUX
    Linux system;
#endif
};

class BasicCase : public ::testing::Test {
protected:
    App app;
};

#endif //BERSERK_TESTBASICCASE_HPP

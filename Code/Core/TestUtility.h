/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TESTUTILITY_H
#define BERSERK_TESTUTILITY_H

#include <Platform/Memory.h>
#include <functional>
#include <stdio.h>
#include <exception>
#include <vector>
#include <string>

namespace Berserk {

    /** Simple testing utility to avoid using GTest (planned to add to the project later) */
    class TestUtility {
    public:

        struct Test {
            std::function<void()> _run;
            std::string _name;
        };

        explicit TestUtility(const char* sectionName) noexcept : sectionName(sectionName) {};
        ~TestUtility() noexcept { runTests(); printStat(); };

        Test& addTest() {
            tests.emplace_back();
            return tests.back();
        }

        void expect(bool cond, const char *message) {
            if (!cond) {
                printf("[BERSERK TEST] ASSERTION FAILED: '%s'\n", message);
                currentTestAssertions += 1;
            }
        }

        void runTests() {
            printf("[BERSERK TEST] Section: '%s'\n", sectionName);
            for (auto& test: tests) {
                currentTestAssertions = 0;
                currentTestName = test._name.c_str();
                auto failed = false;

                printf("[BERSERK TEST] Test: '%s'\n", currentTestName);

                try {
                    test._run();
                } catch (std::exception &e) {
                    printf("[BERSERK TEST] FAILED with exception '%s'\n", e.what());
                    failed = true;
                }

                if (!failed) {
                    if (currentTestAssertions == 0) {
                        printf("[BERSERK TEST] Test: '%s' PASSED\n", currentTestName);
                    } else {
                        printf("[BERSERK TEST] Test: '%s' FAILED (assertions: %u)\n", currentTestName, currentTestAssertions);
                    }
                }

                totalRun += 1;
                totalPassed += (currentTestAssertions ? 0 : 1);
            }
            printf("[BERSERK TEST] Section: '%s'\n", sectionName);
            printf("[BERSERK TEST] TOTAL RUN: %u\n", totalRun);
            printf("[BERSERK TEST] TOTAL PASSED: %u (FAILED: %u)\n", totalPassed, totalRun - totalPassed);

            tests.clear();
        }

        void printStat() {
            auto a = Memory::getAllocCalls();
            auto f = Memory::getFreeCalls();
            printf("[BERSERK TEST] Dyn Memory Stat: alloc: %llu free: %llu\n", a, f);
            if (a != f) {
                printf("[BERSERK TEST] Warning: possible some memory leaks");
            }
        }

    private:

        const char* currentTestName = nullptr;
        uint32 currentTestAssertions = 0;
        uint32 totalRun = 0;
        uint32 totalPassed = 0;
        const char* sectionName;
        std::vector<Test> tests;

    };

}

#endif //BERSERK_TESTUTILITY_H
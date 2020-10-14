/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TESTFIXTURE_HPP
#define BERSERK_TESTFIXTURE_HPP

#include <gtest/gtest.h>
#include <BerserkPlatform/PlatformApplication.hpp>

using namespace Berserk;

class TestFixture : public ::testing::Test {
protected:
    void SetUp() override { mPlatformApplication = new PlatformApplication(0, nullptr); }
    void TearDown() override { delete mPlatformApplication; }

    PlatformApplication* mPlatformApplication = nullptr;
};

#endif //BERSERK_TESTFIXTURE_HPP
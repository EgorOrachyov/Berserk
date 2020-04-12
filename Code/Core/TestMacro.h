/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TESTMACRO_H
#define BERSERK_TESTMACRO_H

#include <TestUtility.h>

#define BERSERK_TEST_SECTION(name) \
    static TestUtility _util( #name ); \
    int main()

#define BERSERK_TEST(name) \
    TestUtility::Test&  _test__ ## name = _util.addTest(); \
    _test__ ## name ._name = #name; \
    _test__ ## name ._run = []() \

#define BERSERK_EXPECT_TRUE(condition) \
    _util.expect((condition), #condition);

#define BERSERK_EXPECT_FALSE(condition) \
    _util.expect(!(condition), #condition);

#endif //BERSERK_TESTMACRO_H

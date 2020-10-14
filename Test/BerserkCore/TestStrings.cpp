/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <TestFixture.hpp>

#include <BerserkCore/Math/Random.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/String/StringBuilder.hpp>

TEST_F(TestFixture, String) {
    const uint32 N = 10;
    const uint32 SEED = 0xf0f435a;

    Random random(SEED);

    float fvalues[N];
    int32 ivalues[N];

    String fstr[N];
    String istr[N];

    for (uint32 i = 0; i < N; i++) {
        fvalues[i] = random.RandomFloatInRange(-1000.f, 1000.f);
        ivalues[i] = random.RandomIntInRange(-1000, 1000);

        fstr[i] = String::ToString(fvalues[i]);
        istr[i] = String::ToString(ivalues[i]);
    }

    for (uint32 i = 0; i < N; i++) {
        EXPECT_EQ(fvalues[i], fstr[i].ToFloat());
        EXPECT_EQ(ivalues[i], istr[i].ToInt32());
    }
}

TEST_F(TestFixture, StringBuilder) {
    StringBuilder builder;

    builder.Append('H');
    builder.Append("ello, ");
    builder.Append(String("World!"));

    builder << " This is " << "some (number: " << 0.52818f << ") fancy builder!";
    String string = builder.ToString();

    printf("%s\n", string.GetStr());
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Misc/Crc32.hpp>
#include <BerserkCore/Strings/String.hpp>

using namespace Berserk;

static const char* sources[] = {
        BERSERK_TEXT("short"),
        BERSERK_TEXT("short-short"),
        BERSERK_TEXT("medium sized string"),
        BERSERK_TEXT("very .......... long ............. string"),
        BERSERK_TEXT("very .......... long ............. string .............. ep"),
        BERSERK_TEXT("короткая"),
        BERSERK_TEXT("короткая-короткая"),
        BERSERK_TEXT("среднего размера строка"),
        BERSERK_TEXT("очень ......... длинная .......... строка"),
        BERSERK_TEXT("очень ......... длинная .......... строка .............. даа"),
};

BERSERK_DEFINE_FIXTURE(StringFixture)

TEST_F(StringFixture, Setup) {
    for (auto source: sources) {
        String string = source;

        EXPECT_TRUE(StringUtils::Compare(string.GetStr(), source) == 0);
    }
}

TEST_F(StringFixture, Concatenation) {
    const char* as[] = {
            BERSERK_TEXT("Abcdef "),
            BERSERK_TEXT("Abcdef dasjno asmofimofieofmlm e30297409  "),
            BERSERK_TEXT("Abcdef 038091241-9- b546-96- =0-=98 =0}}{}{P}{ ")
    };

    const char* bs[] = {
            BERSERK_TEXT("*)лот тфлоыт твфы  "),
            BERSERK_TEXT("ЫДФВщвфы ьдлвфыш йщцз3094 "),
            BERSERK_TEXT("в фышвщ 0293 ьдлфывьдл  двтдфытвщ ")
    };

    const char* cs[] = {
            BERSERK_TEXT("Abcdef ") BERSERK_TEXT("*)лот тфлоыт твфы  "),
            BERSERK_TEXT("Abcdef dasjno asmofimofieofmlm e30297409  ") BERSERK_TEXT("ЫДФВщвфы ьдлвфыш йщцз3094 "),
            BERSERK_TEXT("Abcdef 038091241-9- b546-96- =0-=98 =0}}{}{P}{ ") BERSERK_TEXT("в фышвщ 0293 ьдлфывьдл  двтдфытвщ ")
    };

    uint32 count = sizeof(as) / sizeof(as[0]);

    for (uint32 i = 0; i < count; i++) {
        String a = as[i];
        String b = bs[i];

        String c = a + b;

        EXPECT_TRUE(StringUtils::Compare(c.GetStr(), cs[i]) == 0);

        Equals<String> equals;

        EXPECT_EQ(c == c, equals(c, c));
        EXPECT_EQ(a == b, equals(a, b));
    }
}

TEST_F(StringFixture, Hash) {
    for (auto source: sources) {
        String string = source;

        EXPECT_EQ(string.Hash(), Crc32::Hash(source, StringUtils::Length(source)));

        Hash<String> hash;

        EXPECT_EQ(string.Hash(), hash(string));
    }

    EXPECT_TRUE(true);
}

template <typename Stream>
Stream& operator << (Stream& stream, const String& string) {
    return stream << string.GetStr();
}

TEST_F(StringFixture, Output) {
    for (auto source: sources) {
        String string = source;
        std::setlocale(LC_ALL, "en_US.UTF-8");
        std::cout << string << std::endl;
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
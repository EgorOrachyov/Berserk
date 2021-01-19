/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Misc/Crc32.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Strings/StringName.hpp>
#include <BerserkCore/Strings/Formatter.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Debug/Debug.hpp>

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

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

static const char* nameSources[] = {
        BERSERK_TEXT("LANL 10984 09ndlja nfa sf12=040`3 '/'ds/a f=a;=3=;2-=`90708081203"),
        BERSERK_TEXT("LANL 1fsuhpo 4jh[0рмижщт щткзцт3y209n5y0c9 298hg80708081203"),
        BERSERK_TEXT("ej oijfsuhpo 4jh[0рмижщт щткзцтroij340 t8jg  wkig=[o4 3"),
        BERSERK_TEXT("ej oijfsuhpo 4jh[0рмижщт щткзцтonef[034o=0\32;= 54g g"),
        BERSERK_TEXT("fwkempfsuhpo 4jh[0рмижщт щткзцт r2u 0"),
        BERSERK_TEXT("fwkeывар54 н56 г 1улр0")
};

static const size_t nameCounts[] = {
        1,
        2,
        10,
        150,
        50,
        77
};

BERSERK_DEFINE_FIXTURE(StringFixture)

TEST_F(StringFixture, Setup) {
    for (auto source: sources) {
        String string = source;

        EXPECT_TRUE(StringUtils::Compare(string.GetStr_C(), source) == 0);
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

        EXPECT_TRUE(StringUtils::Compare(c.GetStr_C(), cs[i]) == 0);

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
    return stream << string.GetStr_C();
}

TEST_F(StringFixture, Output) {
    for (auto source: sources) {
        String string = source;
        std::setlocale(LC_ALL, "en_US.UTF-8");
        std::cout << string << std::endl;
    }
}

TEST_F(StringFixture, Formatting) {
    std::setlocale(LC_ALL, "en_US.UTF-8");

    Formatter<> format;

    const char* formats[] = {
            BERSERK_TEXT("Test empty"),
            BERSERK_TEXT("Test with {0} arg"),
            BERSERK_TEXT("Test with {0}, {1} and {0}"),
            BERSERK_TEXT("Test large: | {2} | {1} | {0} | {0} | {1}")
    };

    int32 valueInt32 = 124213;
    int64 valueInt64 = -23452;
    uint32 valueUint32 = 934850;
    uint64 valueUint64 = 123879800003;
    String string1 = BERSERK_TEXT("Some fancy string");
    String string2 = BERSERK_TEXT("\"Another fancy text\"");
    String string3 = BERSERK_TEXT("{very fancy text}");

    {
        std::cout << format.Print(formats[0]) << std::endl;

        std::cout << format.Print(formats[1], valueInt32) << std::endl;
        std::cout << format.Print(formats[1], valueInt64) << std::endl;
        std::cout << format.Print(formats[1], string1) << std::endl;

        std::cout << format.Print(formats[2], valueInt32, valueInt64) << std::endl;
        std::cout << format.Print(formats[2], valueUint32, valueUint64) << std::endl;
        std::cout << format.Print(formats[2], string1, string2) << std::endl;

        std::cout << format.Print(formats[3], string1, valueInt32, valueInt64) << std::endl;
        std::cout << format.Print(formats[3], string2, valueUint32, valueUint64) << std::endl;
        std::cout << format.Print(formats[3], string1, string2, string3) << std::endl;

        std::cout << std::endl;
    }

    {
        Formatter<> fmt;

        String info = fmt.Print("User data: name:\"{0}\", age:{1}, rnd:{2}, uuid:{3}", String("Person"), 22, -0.322f, "af242309fabccc09f");
        String example = fmt.Print("{0} {1} {2} {1} {0}", 231, 3123, Precision<float>(-0.12332123f, 2));

        std::cout << info << std::endl;
        std::cout << example << std::endl;
    }
}

TEST_F(StringFixture, StringsStressTest) {
    uint32 iterations = 10000;
    uint32 step = 10;
    uint32 base = 0;

    Array<String> strings;
    strings.EnsureCapacity(iterations);

    for (auto i = 0; i < iterations; i++) {
        String string(base);
        base += step;

        strings.Move(string);
    }
}

TEST_F(StringFixture, StringName) {
    ASSERT_EQ(ARRAY_SIZE(nameSources), ARRAY_SIZE(nameCounts));

    Array<StringName> strings;

    auto initial = StringNameTable::GetEntriesCount();

    for (auto i = 0; i < ARRAY_SIZE(nameSources); i++) {
        for (auto j = 0; j < nameCounts[i]; j++) {
            strings.Add(nameSources[i]);
        }
    }

    EXPECT_EQ(StringNameTable::GetEntriesCount() - initial, ARRAY_SIZE(nameSources));
}

TEST_F(StringFixture, StringNamePrint) {
    ASSERT_EQ(ARRAY_SIZE(nameSources), ARRAY_SIZE(nameCounts));

    Array<StringName> strings;

    for (auto i = 0; i < ARRAY_SIZE(nameSources); i++) {
        strings.Add(nameSources[i]);
    }

    for (auto& string: strings) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("String name: \"{0}\""), string);
    }
}

BERSERK_GTEST_MAIN
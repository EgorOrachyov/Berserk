/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <TestFixture.hpp>

#include <BerserkCore/Platform/Endian.hpp>
#include <BerserkCore/Math/Random.hpp>
#include <BerserkCore/Logging.hpp>
#include <BerserkCore/LogHtml.hpp>

TEST_F(TestFixture, LogDump) {
    BERSERK_LOG_ALWAYS("Some fancy message");
    BERSERK_LOG_INFO("Some fancy message");
    BERSERK_LOG_WARNING("Some fancy message");
    BERSERK_LOG_ERROR("Some fancy message");

    BERSERK_LOG_WARNING("Warning with: ", 123, " numbers and ", String("strings"));
    BERSERK_LOG_ERROR("Error with ", 0.4f, " ", -0.003, " and more ");

    auto& log = Application::Get().GetLog();
    LogHtml logHtmlDump("log_dump.html", log);
}

TEST_F(TestFixture, Endian) {
    union { char a[4]; uint32 b = 0x07;} test;
    bool isLittleEndian = test.a[0] == 0x07;
    bool isBigEndian = test.a[3] == 0x07;

    EBytesOrder bytesOrder = isLittleEndian? EBytesOrder::LittleEndian: EBytesOrder::BigEndian;

    EXPECT_EQ(isLittleEndian, Endian::IsLittleEndian());
    EXPECT_EQ(isBigEndian, Endian::IsBigEndian());

    EXPECT_EQ(bytesOrder == EBytesOrder::LittleEndian, Endian::NeedToReorder(EBytesOrder::BigEndian));
    EXPECT_EQ(bytesOrder == EBytesOrder::BigEndian, Endian::NeedToReorder(EBytesOrder::LittleEndian));

    const uint32 N = 10;

    Random random(0);

    for (uint32 i = 0; i < N; i++) {
        int32 s = random.RandomInt();
        int32 r = Endian::Reorder(s);

        printf("%x %x \n", s, r);

        EXPECT_EQ(s, Endian::Reorder(r));
    }

    for (uint32 i = 0; i < N; i++) {
        int64 s = random.RandomInt64();
        int64 r = Endian::Reorder(s);

        printf("%llx %llx \n", (long long int)s, (long long int)r);

        EXPECT_EQ(s, Endian::Reorder(r));
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
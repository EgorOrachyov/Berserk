/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Format.h>
#include <TestMacro.h>
#include <Errors.h>
#include <Platform/Memory.h>
#include <Math/Math.h>

using namespace Berserk;

BERSERK_TEST_SECTION(CoreMinimals)
{
    BERSERK_TEST(Format)
    {
        const uint32 bufferSize = 30;
        char buffer[bufferSize] = {0};
        uint32 written = 0;

        written = Format::printf(buffer, bufferSize, "Message");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %f %s", 0.345f, "CString");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %s", "abcabcabcabcabcabcabcabc");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %20s", "Aligned");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %9.5f", 1241.001f);
        printf("%u %s\n", written, buffer);
    };

    BERSERK_TEST(Assert)
    {
        BERSERK_EXPECT_TRUE(1 > 0);
        BERSERK_EXPECT_TRUE(10 - 12 < -100);
        BERSERK_EXPECT_FALSE(100 * 0 == 10);
        BERSERK_EXPECT_FALSE(true);
    };

    BERSERK_TEST(ErrorMacro)
    {
        BERSERK_ERROR("Error without return of abort");
        BERSERK_ERROR("Error without return of abort");
        BERSERK_WARNING("Waring");
        BERSERK_WARNING("Some another waring");

        auto function = [](const char* message, EErrorType errorType) {
            printf("Message: '%s' EErrorType: '%s'\n", message, Errors::getErrorType(errorType));
        };

        Errors::forEachError(function);
    };
}
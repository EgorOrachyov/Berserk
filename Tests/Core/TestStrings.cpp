/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/StringUtility.h>
#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Strings)
{

    BERSERK_TEST(StringUtility)
    {
        using Strings = StringUtility<char, '\0'>;

        char buffer1[100];
        char buffer2[100];

        auto words = "A B C DDDD Something another thing";
        auto find = "Something";
        auto replace1 = "Best think ever";

        Strings::replaceFirst(buffer1, words, find, replace1);
        printf("%s\n", buffer1);

        auto last = "thing";
        auto replace2 = "Looonger";

        Strings::replaceFirst(buffer2, buffer1, last, replace2);
        printf("%s\n", buffer2);
    };

}
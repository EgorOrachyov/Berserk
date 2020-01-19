/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/TStringUtility.h>
#include <String/CStringStatic.h>
#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(Strings)
{

    BERSERK_TEST(StringUtility)
    {
        using Strings = TStringUtility<char, '\0'>;

        char buffer1[100];
        char buffer2[100];

        auto words = "A B C DDDD Something another thing thing thing";

        auto find1 = "Something";
        auto replace1 = "Best think ever";

        auto pos1 = Strings::findFirst(words, find1);
        auto offset1 = Strings::offset(words, pos1);
        Strings::replace(buffer1, 100, words, offset1, Strings::length(find1), replace1);
        printf("%s\n", buffer1);

        auto find2 = "thing";
        auto replace2 = "Looonger";

        auto pos2 = Strings::findLast(buffer1, find2);
        auto offset2 = Strings::offset(buffer1, pos2);
        Strings::replace(buffer2, 100, buffer1, offset2, Strings::length(find2), replace2);
        printf("%s\n", buffer2);
    };

    BERSERK_TEST(CStringStatic)
    {
        CStringStatic hello = "Hello,";
        CStringStatic world = " World!";
        CStringStatic helloworld = hello + world;
        CStringStatic tmp;

        helloworld += " What a nice, nice day!";
        tmp = helloworld;

        tmp.replaceFirst("World", "Universe");
        tmp.replaceLast("nice", "crazy");

        printf("%s\n", tmp.data());

        CStringStatic file = "/some/funny/place.ext";
        CStringStatic ext = "ext";
        CStringStatic some = file + (ext + "_") * 4;

        printf("%s\n", some.data());
    };

}
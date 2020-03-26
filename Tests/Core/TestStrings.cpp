/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <String/TStringUtility.h>
#include <String/CStringStatic.h>
#include <String/CString.h>
#include <String/WString.h>
#include <String/CStringBuilder.h>
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

    BERSERK_TEST(CString)
    {
        printf("CString size: %u\n", (uint32) sizeof(CString));
        printf("CStringStatic size: %u\n", (uint32) sizeof(CStringStatic));

        CString a = "Small string";
        CString b = "Bigger string (requires buffer of more than 20 chars capacity)";

        const auto c = b;
        const auto d = a;

        printf("Data: %s \n", c.data());
        printf("Data: %s \n", d.data());

        auto e = a;
        a += b;
        e += " Append";
        e += " a text";

        printf("Data: %s \n", a.data());
        printf("Data: %s \n", e.data());

        CString root = "./Assets";
        CString textures = "/Textures";
        auto lib = root + "/lib";
        auto dll = lib + "/somedll.dll";
        auto tex = root + textures + "/sometexture.tx";

        printf("Data: %s \n", dll.data());
        printf("Data: %s \n", tex.data());

        CString empty;
        CString small = "aa_";
        CString big = "bb_b*";

        empty = empty * 4;
        small = small * 5;
        big = big * 12;

        printf("Data: %s \n", empty.data());
        printf("Data: %s \n", small.data());
        printf("Data: %s \n", big.data());

        CString text = "/path/to/file._ext_";

        text.replaceFirst("_ext_", "ext");
        printf("Data: %s \n", text.data());

        text.replaceFirst("ext", "_ext_ext_");
        printf("Data: %s \n", text.data());

        text.replaceFirst("path", "path/long");
        printf("Data: %s \n", text.data());

        text.replaceLast("ext", "bigext");
        printf("Data: %s \n", text.data());

        TString<char,'\0',128> long1 = text;
        TString<char,'\0',60> long2;
        long2 = long1 + "append";

        printf("Data: %s \n", long1.data());
        printf("Data: %s \n", long2.data());
    };

    BERSERK_TEST(WString)
    {
        WString a = L"Приветствую тебя мир";
        wprintf(L"%ls %u\n", a.data(), a.length());

        CString b = CString::from(a);
        printf("%s %u\n", b.data(), b.length());

        WString c = WString::from(b);
        wprintf(L"%ls %u\n", c.data(), c.length());
    };

    BERSERK_TEST(CStringBuilder)
    {
        CStringBuilder builder;

        builder.append("Begin builder from sequence ");
        builder.append(CStringStatic(", add sequence"));
        builder.append(CString(", add another string, and "));
        builder += "test += magic!";

        auto s1 = builder.toString();
        auto s2 = builder.toStringStatic();

        printf("CString:       %s\n", s1.data());
        printf("CStringStatic: %s\n", s2.data());
    };

}
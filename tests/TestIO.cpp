/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <Testing.hpp>

#include <core/io/ArgumentParser.hpp>
#include <core/io/Logger.hpp>
#include <platform/Output.hpp>

#include <iostream>

TEST(Berserk, ArgParser) {
    BRK_NS_USE;

    const char *const args[] = {"new", "--path=./some/dir"};

    ArgumentParser parser;
    parser.AddArgument("-help");
    parser.AddArgument("new");
    parser.AddArgument("--path", ".");
    parser.AddArgument("--lang", "cpp");
    parser.Parse(2, args);

    String path;
    String lang;

    EXPECT_TRUE(parser.Set("new"));
    EXPECT_FALSE(parser.Set("-help"));
    EXPECT_TRUE(parser.Set("--path", path));
    EXPECT_FALSE(parser.Set("--lang", lang));
    EXPECT_EQ(path, "./some/dir");
    EXPECT_EQ(lang, "cpp");
}

TEST(Berserk, Logger) {
    auto sink = [](const BRK_NS::Logger::Entry &entry) {
        std::cout << entry.message << " in " << entry.function << " " << entry.file << std::endl;
    };

    BRK_NS::Logger::Instance().AddListener(sink);

    BRK_INFO("Info about " << 10);
    BRK_WARNING("Warning for " << BRK_NS::String("str") << " " << BRK_TEXT("message"));
    BRK_ERROR("Error about " << 10.1f << " and " << 1);
}

TEST(Berserk, Output) {
    BRK_NS_USE;

    Output output;

    output.Write(BRK_TEXT("п"));
    output.Write(BRK_TEXT("Some fancy text привет śążź Ειρήνη zß水🍌\n"));
    output.WriteWarning(BRK_TEXT("Some warn fancy text привет śążź Ειρήνη zß水🍌🍌\n"));
    output.WriteError(BRK_TEXT("Some error fancy text привет śążź Ειρήνη zß水🍌🍌🍌\n"));
}

BRK_GTEST_MAIN
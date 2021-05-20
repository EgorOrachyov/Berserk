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

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>

#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/IO/FileTextWriter.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(FileSystemFixture)

static const char buffer[] = {
    BERSERK_TEXT("Some fancy text ")
    BERSERK_TEXT("About how to work with files ")
    BERSERK_TEXT("На этой платформе ")
    BERSERK_TEXT("На цій дивній платформі ")
    BERSERK_TEXT("この奇妙なプラットフォームで")
};

TEST_F(FileSystemFixture, FileWrite) {
    String data = buffer;
    String filename = "TestFile.txt";

    auto file = FileSystem::OpenFile(filename, File::Mode::Write);

    size_t written = 0;

    if (file) {
        written = file->WriteBytes(data.GetStr_C(), data.GetLength());
        file->Close();
    }

    EXPECT_EQ(written, data.GetLength());
}

TEST_F(FileSystemFixture, FileRead) {
    String data;
    String filename = "TestFile.txt";

    auto file = FileSystem::OpenFile(filename, File::Mode::Read);

    if (file) {
        auto size = file->GetSize();
        String tmp(size + 1);
        file->ReadBytes(tmp.GetStr_C(), size);
        file->Close();

        tmp.GetStr_C()[size] = String::END;
        data = std::move(tmp);
    }

    EXPECT_TRUE(StringUtils::Compare(data.GetStr_C(), buffer) == 0);
}

TEST_F(FileSystemFixture, FileTextWriter) {
    auto file = FileSystem::OpenFile("FileTextWriter.txt", File::Mode::Write);
    Formatter<> ff;
    FileTextWriter writer{file};

    writer.WriteFF(ff, BERSERK_TEXT("This is FileTextWriter test for {0}"), file->GetFilename());
    writer.WriteLine();

    writer.Write(BERSERK_TEXT("Current position is "));
    writer.Write((uint64) file->GetPosition());
    writer.WriteLine();
}

TEST_F(FileSystemFixture, ExecPath) {
    String path = FileSystem::GetExecutablePath();

    printf("%s\n", path.GetStr_C());
}

TEST_F(FileSystemFixture, MakePath) {
    String p1 = FileSystem::MakePath("path");
    String p2 = FileSystem::MakePath("path", "llsdfnl_flsn_s", "sdniomf.txt");
    String p3 = FileSystem::MakePath("path", String("dsfoisnfo"), String("file.name"));
    String p4 = FileSystem::MakePath(String("dlksm"), "asdfsaf", "dsfsd", "fdsfsdfaf", "fsdfsdfsd", "fsdf");

    printf("%s\n", p1.GetStr_C());
    printf("%s\n", p2.GetStr_C());
    printf("%s\n", p3.GetStr_C());
    printf("%s\n", p4.GetStr_C());
}

TEST_F(FileSystemFixture, FileFromPath) {
    Array<String> paths = {
        BERSERK_TEXT("abcd.efg"),
        BERSERK_TEXT("abcd.efg/"),
        BERSERK_TEXT("askldmn/gSLDNF_ASDL.ldasndkS"),
        BERSERK_TEXT("askldmn/OIEFH_SDFLK___19849238ad/MLIJGELS.SA____ASD.ASD"),
        BERSERK_TEXT("askldmn/OIEFH_SDFLK___19849238ad/LkNFNEOAFNLKNEDKLFM_FLNSDJLNF___OF_SDF/mlksafd.ASDL__________SAD.AD")
    };

    Array<String> names = {
        BERSERK_TEXT("abcd.efg"),
        BERSERK_TEXT(""),
        BERSERK_TEXT("gSLDNF_ASDL.ldasndkS"),
        BERSERK_TEXT("MLIJGELS.SA____ASD.ASD"),
        BERSERK_TEXT("mlksafd.ASDL__________SAD.AD")
    };

    ASSERT_EQ(paths.GetSize(), names.GetSize());

    for (Berserk::size_t i = 0; i < paths.GetSize(); i++) {
        auto name = FileSystem::GetFileNameFromPathUnix(paths[i]);
        EXPECT_TRUE(name == names[i]);
    }
}

BERSERK_GTEST_MAIN
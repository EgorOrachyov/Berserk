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
#include <BerserkCore/Platform/Path.hpp>

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

TEST_F(FileSystemFixture, GetCwd) {
    auto currentDir = FileSystem::GetCurrentDirectory();
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Current working directory {0}"), currentDir);
}

TEST_F(FileSystemFixture, SetCwd) {
    auto currentDir = FileSystem::GetCurrentDirectory();
    auto newDir = FileSystem::MakePath(currentDir, "..", "..");
    auto absDir = FileSystem::GetAbsolutePath(newDir);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Current working directory {0}"), currentDir);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("New working directory {0}: {1}"), absDir, FileSystem::SetCurrentDirectory(absDir));
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Current working directory {0}"), FileSystem::GetCurrentDirectory());
}

TEST_F(FileSystemFixture, Abs) {
    auto basePath = FileSystem::GetCurrentDirectory();
    auto absPath = FileSystem::GetAbsolutePath(FileSystem::MakePath(basePath, "..", ".."));
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Base directory {0}"), basePath);
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Absolute path {0}"), absPath);
}

TEST_F(FileSystemFixture, Home) {
    auto homeDir = FileSystem::GetHomeDirectory();
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Home directory {0}"), homeDir);
}

TEST_F(FileSystemFixture, DirList) {
    auto currentDir = FileSystem::GetCurrentDirectory();
    Array<FileSystem::Entry> entries;

    FileSystem::ListDirectory(currentDir, entries);

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("List directory {0}"), currentDir);
    for (auto& item: entries)
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("- {0} type={1}"), item.name, item.type);
}

TEST_F(FileSystemFixture, DirManagement) {
    auto home = FileSystem::GetHomeDirectory();
    auto berserk = FileSystem::MakePath(home, BERSERK_TEXT(".berserk"));

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Check directory {0}: {1}"), berserk, FileSystem::Exists(berserk));
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Create directory {0}: {1}"), berserk, FileSystem::CreateDirectory(berserk));

    if (FileSystem::Exists(berserk)) {
        BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Remove directory {0}: {1}"), berserk, FileSystem::RemoveEntry(berserk));
    }
}

TEST_F(FileSystemFixture, PathUnix) {
    Path p1(BERSERK_TEXT("/home/string/name"), FileSystem::PathType::Unix);
    EXPECT_TRUE(p1.HasFilename());
    EXPECT_TRUE(!p1.HasDevice());
    EXPECT_TRUE(p1.IsAbsolute());
    EXPECT_TRUE(p1.MakePathUnix() == BERSERK_TEXT("/home/string/name"));

    Path p2(BERSERK_TEXT("/home/string/name/"), FileSystem::PathType::Unix);
    EXPECT_TRUE(!p2.HasFilename());
    EXPECT_TRUE(!p2.HasDevice());
    EXPECT_TRUE(p2.IsAbsolute());
    EXPECT_TRUE(p2.MakePathUnix() == BERSERK_TEXT("/home/string/name/"));

    p2.SetFilename("file");
    EXPECT_TRUE(p2.MakePathUnix() == BERSERK_TEXT("/home/string/name/file"));

    Path p3(BERSERK_TEXT("/home/////string//////name///////file"), FileSystem::PathType::Unix);
    EXPECT_TRUE(p3.HasFilename());
    EXPECT_TRUE(!p3.HasDevice());
    EXPECT_TRUE(p3.IsAbsolute());
    EXPECT_TRUE(p3.MakePathUnix() == BERSERK_TEXT("/home/string/name/file"));

    Path p4(BERSERK_TEXT("~//////"), FileSystem::PathType::Unix);
    EXPECT_TRUE(p4.IsAbsolute());
    EXPECT_TRUE(p4.MakePathUnix() == BERSERK_TEXT("~/"));

    Path p5(BERSERK_TEXT("/home/user/directory/file"), FileSystem::PathType::Unix);

    auto pp5 = p5.Parent();
    EXPECT_TRUE(pp5.MakePathUnix() == BERSERK_TEXT("/home/user/directory/"));
    auto ppp5 = pp5.Parent();
    EXPECT_TRUE(ppp5.MakePathUnix() == BERSERK_TEXT("/home/user/"));
    auto pppp5 = ppp5.Parent();
    EXPECT_TRUE(pppp5.MakePathUnix() == BERSERK_TEXT("/home/"));
    auto ppppp5 = pppp5.Parent();
    EXPECT_TRUE(ppppp5.MakePathUnix() == BERSERK_TEXT("/"));
}

TEST_F(FileSystemFixture, PathWindows) {
    Path p1(BERSERK_TEXT("\\home\\string\\name"), FileSystem::PathType::Windows);
    EXPECT_TRUE(p1.HasFilename());
    EXPECT_TRUE(!p1.HasDevice());
    EXPECT_TRUE(p1.IsAbsolute());
    EXPECT_TRUE(p1.MakePathWindows() == BERSERK_TEXT("\\home\\string\\name"));

    Path p2(BERSERK_TEXT("\\home\\string\\name\\"), FileSystem::PathType::Windows);
    EXPECT_TRUE(!p2.HasFilename());
    EXPECT_TRUE(!p2.HasDevice());
    EXPECT_TRUE(p2.IsAbsolute());
    EXPECT_TRUE(p2.MakePathWindows() == BERSERK_TEXT("\\home\\string\\name\\"));

    p2.SetFilename("file");
    EXPECT_TRUE(p2.MakePathWindows() == BERSERK_TEXT("\\home\\string\\name\\file"));

    Path p3(BERSERK_TEXT("home\\\\\\string\\\\\\\\\\\\name\\\\\\file"), FileSystem::PathType::Windows);
    EXPECT_TRUE(p3.HasFilename());
    EXPECT_TRUE(!p3.HasDevice());
    EXPECT_TRUE(!p3.IsAbsolute());
    EXPECT_TRUE(p3.MakePathWindows() == BERSERK_TEXT("home\\string\\name\\file"));

    Path p4(BERSERK_TEXT("C:\\"), FileSystem::PathType::Windows);
    EXPECT_TRUE(p4.IsAbsolute());
    EXPECT_TRUE(p4.MakePathWindows() == BERSERK_TEXT("C:\\"));

    Path p5(BERSERK_TEXT("C:\\home\\user\\directory\\file"), FileSystem::PathType::Windows);

    auto pp5 = p5.Parent();
    EXPECT_TRUE(pp5.MakePathWindows() == BERSERK_TEXT("C:\\home\\user\\directory\\"));
    auto ppp5 = pp5.Parent();
    EXPECT_TRUE(ppp5.MakePathWindows() == BERSERK_TEXT("C:\\home\\user\\"));
    auto pppp5 = ppp5.Parent();
    EXPECT_TRUE(pppp5.MakePathWindows() == BERSERK_TEXT("C:\\home\\"));
    auto ppppp5 = pppp5.Parent();
    EXPECT_TRUE(ppppp5.MakePathWindows() == BERSERK_TEXT("C:\\"));
}

TEST_F(FileSystemFixture, PathCreate) {
    Path path = FileSystem::GetHomeDirectory();
    path.SetFilenameAsDirectory();

    path.AppendDir(BERSERK_TEXT(".berserk"));
    if (!FileSystem::Exists(path.MakePath()))
        FileSystem::CreateDirectory(path.MakePath());

    path.AppendDir(BERSERK_TEXT("cache"));
    if (!FileSystem::Exists(path.MakePath()))
        FileSystem::CreateDirectory(path.MakePath());

    path.SetFilename(BERSERK_TEXT("312-3123-12314-234.txt"));
    if (!FileSystem::Exists(path.MakePath())) {
        auto filePath = path.MakePath();
        auto file = FileSystem::OpenFile(filePath, File::Mode::Write);
        auto text = String(BERSERK_TEXT("Some text to save"));

        file->WriteBytes(text.GetStr_C(), text.GetLength());
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Result path: {0}"), path);
}

TEST_F(FileSystemFixture, PathRemove) {
    Path path = FileSystem::GetHomeDirectory();
    path.SetFilenameAsDirectory();

    path.AppendDir(BERSERK_TEXT(".berserk"));
    path.AppendDir(BERSERK_TEXT("cache"));
    path.SetFilename(BERSERK_TEXT("312-3123-12314-234.txt"));

    if (FileSystem::Exists(path.MakePath())) {
        // FileSystem::RemoveEntry(path.MakePath());
        path = path.Parent();
    }

    if (FileSystem::Exists(path.MakePath())) {
        // FileSystem::RemoveEntry(path.MakePath());
        path = path.Parent();
    }

    if (FileSystem::Exists(path.MakePath())) {
        // FileSystem::RemoveEntry(path.MakePath());
        path = path.Parent();
    }

    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Result path: {0}"), path);
}


BERSERK_GTEST_MAIN
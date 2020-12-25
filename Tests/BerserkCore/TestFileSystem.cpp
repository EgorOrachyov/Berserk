/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>

#include <BerserkCore/Containers/Array.hpp>
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

    PtrShared<Platform::File> file = Platform::FileSystem::OpenFile(filename, Platform::File::Mode::Write);

    size_t written = 0;

    if (file) {
        written = file->WriteBytes(data.GetStr(), data.GetLength() + 1);
        file->Close();
    }

    EXPECT_EQ(written, data.GetLength() + 1);
}

TEST_F(FileSystemFixture, FileRead) {
    String data;
    String filename = "TestFile.txt";

    PtrShared<Platform::File> file = Platform::FileSystem::OpenFile(filename, Platform::File::Mode::Read);

    if (file) {
        auto size = file->GetSize();
        String tmp(size);
        file->ReadBytes(tmp.GetStr(), size);
        file->Close();

        data = std::move(tmp);
    }

    EXPECT_TRUE(StringUtils::Compare(data.GetStr(), buffer) == 0);
}

TEST_F(FileSystemFixture, ExecPath) {
    String path = Platform::FileSystem::GetExecutablePath();

    printf("%s\n", path.GetStr());
}

TEST_F(FileSystemFixture, MakePath) {
    String p1 = Platform::FileSystem::MakePath("path");
    String p2 = Platform::FileSystem::MakePath("path", "llsdfnl_flsn_s", "sdniomf.txt");
    String p3 = Platform::FileSystem::MakePath("path", String("dsfoisnfo"), String("file.name"));
    String p4 = Platform::FileSystem::MakePath(String("dlksm"), "asdfsaf", "dsfsd", "fdsfsdfaf", "fsdfsdfsd", "fsdf");

    printf("%s\n", p1.GetStr());
    printf("%s\n", p2.GetStr());
    printf("%s\n", p3.GetStr());
    printf("%s\n", p4.GetStr());
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
        auto name = Platform::FileSystem::GetFileNameFromPathUnix(paths[i]);
        EXPECT_TRUE(name == names[i]);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
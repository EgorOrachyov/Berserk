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

#include <platform/FileSystem.hpp>

#include <iostream>

TEST(Berserk, GetExecutablePath) {
    BRK_NS_USE

    FileSystem fs;

    std::cout << fs.GetExecutablePath() << std::endl;
}

TEST(Berserk, GetFullPath) {
    BRK_NS_USE

    FileSystem fs;

    auto path = fs.GetExecutablePath();
    auto searchPath = path.substr(0, path.find_last_of('/'));

    fs.AddSearchPath(searchPath);

    std::cout << searchPath << std::endl;
    std::cout << fs.GetFullFilePath("TestFileSystem.exe") << std::endl;
    std::cout << fs.GetFullFilePath("TestFileSystem") << std::endl;
    std::cout << fs.GetFullDirPath("CMakeFiles") << std::endl;
}

TEST(Berserk, GetFileExtension) {
    BRK_NS_USE

    FileSystem fs;

    std::cout << fs.GetFileExtension("some/fancy/ext/GetFileExtension.ext");
}

TEST(Berserk, GetFileName) {
    BRK_NS_USE

    FileSystem fs;

    std::cout << fs.GetFileName("some/very/fancy/GetFileName.ext");
}

TEST(Berserk, ListDir) {
    BRK_NS_USE

    FileSystem fs;

    auto path = fs.GetExecutablePath();
    auto searchPath = path.substr(0, path.find_last_of('/'));

    fs.AddSearchPath(searchPath);

    auto entries = fs.ListDir(".");
    for (auto &entry : entries)
        std::cout << entry.name << " "
                  << (entry.type == BRK_NS::FileSystem::EntryType::File ? "file" : "dir") << std::endl;
}

TEST(Berserk, OpenFile) {
    BRK_NS_USE

    FileSystem fs;

    auto path = fs.GetExecutablePath();
    auto searchPath = path.substr(0, path.find_last_of('/'));

    fs.AddSearchPath(searchPath);

    if (std::FILE *file = fs.OpenFile(fs.GetFullFilePath("cmake_install.cmake"), "r")) {
        auto count = 1000;

        std::vector<char> text(count + 1, '\0');
        std::fread(text.data(), 1, count, file);
        std::fclose(file);

        std::cout << text.data() << std::endl;
    }
}

TEST(Berserk, ReadFile) {
    BRK_NS_USE

    FileSystem fs;

    auto path = fs.GetExecutablePath();
    auto searchPath = path.substr(0, path.find_last_of('/'));

    fs.AddSearchPath(searchPath);

    auto data = fs.ReadFile(fs.GetFullFilePath("cmake_install.cmake"));

    if (data.IsNotNull()) {
        std::cout << String(reinterpret_cast<const char *>(data->GetData()), data->GetSize()) << std::endl;
    }
}

BRK_GTEST_MAIN
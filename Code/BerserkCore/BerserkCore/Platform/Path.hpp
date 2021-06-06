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

#ifndef BERSERK_PATH_HPP
#define BERSERK_PATH_HPP

#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/Array.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>
#include <BerserkCore/Templates/Contracts.hpp>

namespace Berserk {

    class Path {
    public:
        Path() = default;
        Path(const char* path, FileSystem::PathType pathType = FileSystem::PathType::Current);
        Path(const String& path, FileSystem::PathType pathType = FileSystem::PathType::Current);
        Path(const Path& other) = default;
        Path(Path&& other) noexcept = default;
        ~Path() = default;

        Path& operator=(const Path& other) = default;
        Path& operator=(Path&& other) noexcept = default;

        Path Parent() const;

        void AppendDir(String directory);
        void SetFilename(String filename);
        void SetDevice(String device);
        void SetFilenameAsDirectory();

        const Array<String> &GetDirectories() const { return mDirectories; }
        const String &GetFilename() const { return mFilename; }
        const String &GetDevice() const { return mDevice; }

        bool HasFilename() const { return mFilename.GetLength() != 0; }
        bool HasDevice() const { return mDevice.GetLength() != 0; }
        bool IsAbsolute() const { return mAbsolute; }

        String MakePath() const;
        String MakePathUnix() const;
        String MakePathWindows() const;

    private:
        void ParseUnix(const char* path);
        void ParseWindows(const char* path);

        Array<String> mDirectories;
        String mDevice;
        String mFilename;
        bool mAbsolute = false;
    };

    template<>
    class TextPrint<Path> {
    public:
        template<typename Stream>
        void operator()(Stream& stream, const Path& path) const {
            stream.Add(path.MakePath());
        }
    };

}

#endif //BERSERK_PATH_HPP
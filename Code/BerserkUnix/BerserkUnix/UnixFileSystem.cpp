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

#include <BerserkUnix/UnixFileSystem.hpp>
#include <BerserkUnix/UnixFile.hpp>
#include <whereami.h>

namespace Berserk {

    UnixFileSystem::UnixImpl::UnixImpl() {
        // Get absolute path to executable
        {
            int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
            String path(pathLength + 1);
            wai_getExecutablePath(path.GetStr_C(), pathLength, nullptr);
            path.GetStr_C()[pathLength] = '\0';
            mExecutablePath = std::move(path);
        }

        Provide(this);
    }

    UnixFileSystem::UnixImpl::~UnixImpl() {
        Remove(this);
    }

    const String &UnixFileSystem::UnixImpl::GetExecutablePath() {
        return mExecutablePath;
    }

    SharedPtr<File> UnixFileSystem::UnixImpl::OpenFile(const String &filepath, File::Mode mode) {
        UnixFile unixFile(filepath, mode);

        if (unixFile.IsOpen())
            return  (SharedPtr<File>) SharedPtr<UnixFile>::MakeMove(std::move(unixFile));

        return {};
    }

}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkLinux/LinuxFileSystem.hpp>
#include <BerserkLinux/LinuxFile.hpp>
#include <whereami.h>

namespace Berserk {
    namespace Platform {

        LinuxFileSystem::LinuxImpl::LinuxImpl() {
            // Get absolute path to executable
            {
                int32 pathLength = wai_getExecutablePath(nullptr, 0, nullptr);
                String path(pathLength + 1);
                wai_getExecutablePath(path.GetStr(), pathLength, nullptr);
                path.GetStr()[pathLength] = '\0';
                mExecutablePath = std::move(path);
            }

            Provide(this);
        }

        LinuxFileSystem::LinuxImpl::~LinuxImpl() noexcept {
            Remove(this);
        }

        const String &LinuxFileSystem::LinuxImpl::GetExecutablePath() {
            return mExecutablePath;
        }

        PtrShared<File> LinuxFileSystem::LinuxImpl::OpenFile(const String &filepath, File::Mode mode) {
            LinuxFile file(filepath, mode);
            return file.IsOpen()? (PtrShared<File>) PtrShared<LinuxFile>::MakeMove(std::move(file)): PtrShared<File>();
        }

    }
}
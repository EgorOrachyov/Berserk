/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkUnix/UnixFileSystem.hpp>
#include <BerserkUnix/UnixFile.hpp>
#include <whereami.h>

namespace Berserk {
    namespace Platform {

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

        UnixFileSystem::UnixImpl::~UnixImpl() noexcept {
            Remove(this);
        }

        const String &UnixFileSystem::UnixImpl::GetExecutablePath() {
            return mExecutablePath;
        }

        Ref<File> UnixFileSystem::UnixImpl::OpenFile(const String &filepath, File::Mode mode) {
            return (Ref<File>) UnixFile::Create(filepath, mode);
        }

    }
}
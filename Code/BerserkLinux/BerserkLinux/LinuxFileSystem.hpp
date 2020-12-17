/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXFILESYSTEM_HPP
#define BERSERK_LINUXFILESYSTEM_HPP

#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {
    namespace Platform {
        
        class LinuxFileSystem: public FileSystem {
        public:

            class LinuxImpl: public FileSystem::Impl {
            public:
                LinuxImpl();
                ~LinuxImpl() override;
                const String &GetExecutablePath() override;
                PtrShared<File> OpenFile(const String &filepath, File::Mode mode) override;

            private:
                String mExecutablePath;
            };

        };
        
    }
}

#endif //BERSERK_LINUXFILESYSTEM_HPP
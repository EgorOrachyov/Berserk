/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXFILESYSTEM_HPP
#define BERSERK_UNIXFILESYSTEM_HPP

#include <BerserkCore/Platform/FileSystem.hpp>

namespace Berserk {
    namespace Platform {
        
        class UnixFileSystem: public FileSystem {
        public:

            class UnixImpl: public FileSystem::Impl {
            public:
                UnixImpl();
                ~UnixImpl() override;
                const String &GetExecutablePath() override;
                Ref<File> OpenFile(const String &filepath, File::Mode mode) override;

            private:
                String mExecutablePath;
            };

        };
        
    }
}

#endif //BERSERK_UNIXFILESYSTEM_HPP
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUX_HPP
#define BERSERK_LINUX_HPP

#include <BerserkCore/Platform/System.hpp>
#include <BerserkPlatform/PlatformAllocator.hpp>

namespace Berserk {

    class Linux : public System {
    public:
        Linux();
        ~Linux() override = default;

        Allocator &GetAllocator() override;
        DateTime GetDateTime() override;
        TPtrShared<File> OpenFile(String filePath, EFileMode fileMode) override;
        const String &GetExecutablePath() const override;

    private:
        void ExtractExePath();

        PlatformAllocator mAllocator;
        String mExecutablePath;
    };

}

#endif //BERSERK_LINUX_HPP
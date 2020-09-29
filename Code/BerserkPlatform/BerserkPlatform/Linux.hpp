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

namespace Berserk {

    class Linux : public System {
    public:
        Linux();
        ~Linux() override = default;

        void OnWarning(const char *message) override;
        void OnError(const char *message) override;
        void OnFatalError(const char *message) override;

        TPtrShared<File> OpenFile(String filePath, EFileMode fileMode) override;

        const String &GetExecutablePath() const override;

    private:
        void ExtractExePath();

        String mExecutablePath;
    };

}

#endif //BERSERK_LINUX_HPP
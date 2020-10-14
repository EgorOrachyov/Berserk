/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MACOS_HPP
#define BERSERK_MACOS_HPP

#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    class MacOS final: public System {
    public:
        MacOS();
        ~MacOS() override;

        TPtrShared<File> OpenFile(String filePath, EFileMode fileMode) override;

        const String &GetExecutablePath() const override;

    private:
        void ExtractExePath();

        String mExecutablePath;
    };

}

#endif //BERSERK_MACOS_HPP
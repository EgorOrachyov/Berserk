/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MACSYSTEM_HPP
#define BERSERK_MACSYSTEM_HPP

#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    class MacSystem final: public System {
    public:

        ~MacSystem() override;

        void OnWarning(const char *message) override;

        void OnError(const char *message) override;

        void OnFatalError(const char *message) override;

        TPtrShared<File> OpenFile(String filePath, EFileMode fileMode) override;

        const String &GetExecutablePath() const override;

    private:

        String mExecutablePath;
    };

}

#endif //BERSERK_MACSYSTEM_HPP
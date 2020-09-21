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

        void onWarning(const char *message) override;

        void onError(const char *message) override;

        void onFatalError(const char *message) override;

        PtrShared<File> openFile(String filePath, EFileMode fileMode) override;

        const String &getExecutablePath() const override;

    private:

        String mExecutablePath;
    };

}

#endif //BERSERK_MACSYSTEM_HPP
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Platform/Platform.hpp>
#include <BerserkCore/String/TString.hpp>
#include <BerserkCore/Platform/File.hpp>
#include <BerserkCore/TPtrShared.hpp>

namespace Berserk {

    /** Abstracts underlying OS specifics */
    class System {
    public:
        virtual ~System() = default;
        virtual void onWarning(const char* message) = 0;
        virtual void onError(const char* message) = 0;
        virtual void onFatalError(const char* message) = 0;
        virtual TPtrShared<File> openFile(String filePath, EFileMode fileMode) = 0;
        //virtual class Directory* openDirectory() = 0;
        //virtual class Library* openLibrary() = 0;
        virtual const String& getExecutablePath() const = 0;
    };

}

#endif //BERSERK_SYSTEM_HPP
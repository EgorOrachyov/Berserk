/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_APPLICATION_HPP
#define BERSERK_APPLICATION_HPP

#include <BerserkCore/Log.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    /** Engine Core application, always accessible for engine modules */
    class Application {
    public:
        Application();
        virtual ~Application();

        virtual Log& GetLog() = 0;
        virtual System& GetSystem() = 0;

        virtual const TArray<String>& GetCmdLineArgs() = 0;
        virtual const String& GetAppName() = 0;
        virtual const String& GetAuthorName() = 0;
        virtual const String& GetCredits() = 0;
        virtual const String& GetExecDir() = 0;
        virtual const String& GetBaseDir() = 0;
        virtual const String& GetResourcesDir() = 0;
        virtual const String& GetEngineDir() = 0;
        virtual const String& GetEditorDir() = 0;

        virtual void Abort(const char* message, uint32 length) = 0;
        virtual void RequestExit() = 0;
        virtual void EnterMainLoop() = 0;

        static Application& Get();

    private:
        static Application* gApplication;
    };

}

#endif //BERSERK_APPLICATION_HPP
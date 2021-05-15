/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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
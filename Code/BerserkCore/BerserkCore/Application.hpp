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
#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Allocator.hpp>

namespace Berserk {

    class Application {
    public:
        Application();
        ~Application() = default;
        Log& GetLog() const { return *mLog; }
        System& GetSystem() const { return *mSystem; }
        Allocator& GetGlobalAllocator() const { return *mGlobalAllocator; }
        static Application& GetSingleton();

    protected:
        Log* mLog = nullptr;
        System* mSystem = nullptr;
        Allocator* mGlobalAllocator = nullptr;

        static Application* gApplication;
    };

}

#endif //BERSERK_APPLICATION_HPP
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
        Log& getLog() const { return *mLog; }
        System& getSystem() const { return *mSystem; }
        Allocator& getGlobalAllocator() const { return *mGlobalAllocator; }
        static Application& getSingleton();

    private:
        Log* mLog;
        System* mSystem;
        Allocator* mGlobalAllocator;
        static Application* gApplication;
    };

}

#endif //BERSERK_APPLICATION_HPP
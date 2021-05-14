/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLFWCONTEXT_HPP
#define BERSERK_GLFWCONTEXT_HPP

#include <BerserkGlfw/GlfwWindowManager.hpp>

namespace Berserk {

    class GlfwContext {
    public:
        GlfwContext(bool useVsync);
        ~GlfwContext();

        void Update();

    private:
        friend class GlfwWindowManager;

        RecursiveMutex& GetMutex() const;

        /** Glfw callback */
        static void ErrorCallback(int32 errorCode, const char *description);

        /** Store and manage application windows */
        GlfwWindowManager::GlfwImpl* mWindowManager = nullptr;

        /** Access for glfw functions */
        mutable RecursiveMutex mMutex;
    };
}




#endif //BERSERK_GLFWCONTEXT_HPP

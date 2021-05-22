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

#ifndef BERSERK_GLFWCONTEXT_HPP
#define BERSERK_GLFWCONTEXT_HPP

#ifdef BERSERK_WITH_VULKAN
#include <vulkan/vulkan.h>
#endif

#include <BerserkGlfw/GlfwWindowManager.hpp>

namespace Berserk {

    class GlfwContext {
    public:
        GlfwContext(bool useVsync, bool noClientApi);
        ~GlfwContext();

        void Update();

#ifdef BERSERK_WITH_VULKAN
        Array<String> GetRequiredInstanceExt() const;
        SharedPtr<Window> GetPrimaryWindow() const;
        Function<VkResult(VkInstance,const SharedPtr<Window>&, VkSurfaceKHR&)> GetSurfaceCreationFunction();
#endif

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

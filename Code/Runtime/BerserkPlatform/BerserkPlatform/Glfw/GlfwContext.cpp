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

#include <BerserkPlatform/Glfw/GlfwContext.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Defines.hpp>

namespace Berserk {

    GlfwContext::GlfwContext(bool useVsync, bool noClientApi) {
        glfwInit();
        glfwSetErrorCallback(ErrorCallback);

    #if BERSERK_PLATFORM == BERSERK_PLATFORM_MACOS
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #elif BERSERK_PLATFORM == BERSERK_PLATFORM_LINUX
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #elif BERSERK_PLATFORM == BERSERK_PLATFORM_WINDOWS
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        if (noClientApi) {
            // For Vulkan based renderer
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        mWindowManager = Memory::Make<GlfwWindowManager::GlfwImpl>(*this, useVsync, noClientApi);
    }

    GlfwContext::~GlfwContext() {
        Memory::Release(mWindowManager);

        glfwTerminate();
    }

    void GlfwContext::Update() {
        Guard<RecursiveMutex> guard(mMutex);

        // Before events polled and callbacks are called
        mWindowManager->PreUpdate();

        // Actual on glfw side update
        glfwPollEvents();

        // Minor actions after new info got
        mWindowManager->PostUpdate();
    }

#ifdef BERSERK_WITH_VULKAN
    Array<String> GlfwContext::GetRequiredInstanceExt() const {

        Array<String> extensions;
        const char** glfwExtensions;
        uint32 extensionsCount;

        {
            Guard<RecursiveMutex> guard(mMutex);
            glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
        }

        if (extensionsCount > 0) {
            extensions.EnsureToAdd(extensionsCount);

            for (uint64 i = 0; i < extensionsCount; i++) {
                extensions.Add(glfwExtensions[i]);
            }
        }

        return extensions;
    }

    SharedPtr<Window> GlfwContext::GetPrimaryWindow() const {
        return mWindowManager->GetPrimaryWindow();
    }

    Function<VkResult(VkInstance, const SharedPtr<Window> &, VkSurfaceKHR &)> GlfwContext::GetSurfaceCreationFunction() {
        auto factory = [this](VkInstance instance, const SharedPtr<Window> & window, VkSurfaceKHR &surface) -> VkResult {
            assert(window);
            assert(instance);

            auto native = dynamic_cast<GlfwWindow*>(window.Get());

            /** We try to access glfw function, so we lock here */
            Guard<RecursiveMutex> guard(mMutex);
            GLFWwindow* handle = native->GetNativeHandle();

            return glfwCreateWindowSurface(instance, handle, nullptr, &surface);
        };

        return factory;
    }
#endif

    RecursiveMutex & GlfwContext::GetMutex() const {
        return mMutex;
    }

    void GlfwContext::ErrorCallback(int32 errorCode, const char *description) {
        BERSERK_LOG_ERROR(BERSERK_TEXT("Glfw"), BERSERK_TEXT("ErrorCallback: code={0} text={1}"), errorCode, description);
    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkGlfw/GlfwContext.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Defines.hpp>

namespace Berserk {
    namespace Platform {

        GlfwContext::GlfwContext() {
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
        #else
            #error "Unsupported platform"
        #endif

            mWindowManager = Create<GlfwWindowManager::GlfwImpl>(*this);
        }

        GlfwContext::~GlfwContext() {
            Release(mWindowManager);

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

        RecursiveMutex & GlfwContext::GetMutex() const {
            return mMutex;
        }

        void GlfwContext::ErrorCallback(int32 errorCode, const char *description) {
            BERSERK_LOG_ERROR(BERSERK_TEXT("Glfw"), BERSERK_TEXT("ErrorCallback: code={0} text={1}"), errorCode, description);
        }

    }
}
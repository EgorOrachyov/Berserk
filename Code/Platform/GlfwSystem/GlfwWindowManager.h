/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOWMANAGER_H
#define BERSERK_GLFWWINDOWMANAGER_H

#include <GlfwSystem/GlfwWindow.h>
#include <Platform/WindowManager.h>

namespace Berserk {

    class GlfwWindowManager final: public WindowManager {
    public:

        GlfwWindowManager();
        ~GlfwWindowManager() override;

        TPtrShared <GlfwWindow> createInternal(const CString &name, const CString &caption, Size2i size,
                                               EWindowActionOnClose action);
        TPtrShared<Window> create(const CString &name, const CString &caption, Size2i size) override;
        TPtrShared<Window> find(const CString &name) override;
        TPtrShared<Window> getFocusedWindow() override;
        bool contains(const CString &name) const override;

        void setErrorFunction(Function<void(const char* what)> function);
        void initializeForMacOS();
        void forceVSync();
        void pollEvents();
        void finalize();
        TArray<TPtrShared<GlfwWindow>> &getWindows() { return mWindows; }
        Function<void(const char* what)> &getOnErrorFunc() { return mOnError; }
        TPtrShared<GlfwWindow> findWindowByHandle(GLFWwindow* hnd);

        static void framebufferSizeCallback(GLFWwindow* handle, int32 width, int32 height);
        static void iconifyCallback(GLFWwindow* handle, int iconify);
        static void positionCallback(GLFWwindow* handle, int posX, int posY);
        static void focusCallback(GLFWwindow* handle, int focus);
        static void errorCallback(int errorCode, const char *description);
        static GlfwWindowManager& getGlfwManager();

    private:

        TArray<TPtrShared<GlfwWindow>> mWindows;
        Function<void(const char* what)> mOnError;

    };

}

#endif //BERSERK_GLFWWINDOWMANAGER_H
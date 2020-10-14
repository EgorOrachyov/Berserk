/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLFWWINDOW_HPP
#define BERSERK_GLFWWINDOW_HPP

#include <BerserkCore/Platform/Window.hpp>
#include <BerserkCore/Platform/WindowManager.hpp>
#include <BerserkCore/Containers/TArray.hpp>
#include <GLFW/glfw3.h>

namespace Berserk {

    class GlfwWindow final: public Window {
    public:
        GlfwWindow(class GlfwWindowManager* manager, WindowDesc windowDesc);
        ~GlfwWindow() override;

        void RequestFocus() override;

        void AddListener(WindowEventListener &listener) override;
        void RemoveListener(WindowEventListener &listener) override;

        EWindowVisibility GetVisibility() const override;
        Point2i GetPosition() const override;
        Size2i GetSize() const override;
        Vec2f GetPixelRatio() const override;
        bool IsInFocus() const override;
        const String &GetName() const override;
        const String &GetTitle() const override;

        void OnFramebufferSizeChanged(const Size2i &newFramebufferSize);
        void OnWindowMoved(const Point2i &newPosition);
        void OnWindowResized(const Size2i &newSize);
        void OnContentScaleChanged(const Vec2f &newScale);
        void OnFocusChanged(bool isInFocus);
        void OnMinimized(bool minimized);
        void OnClose();

        GLFWwindow* GetHND() const { return mWindowHND; }
        WindowEvent GetEventBase(EWindowEventType type) const;

    private:
        GLFWwindow* mWindowHND;
        String mName;
        String mTitle;
        Point2i mPosition;
        Size2i mSize;
        Size2i mFramebufferSize;
        Vec2f mPixelRatio;
        bool mIsInFocus;
        EWindowVisibility mVisibility;
        TArray<WindowEventListener*> mListeners;

        class GlfwWindowManager* mManager;
    };

}

#endif //BERSERK_GLFWWINDOW_HPP
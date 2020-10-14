/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkPlatform/GlfwWindow.hpp>
#include <BerserkPlatform/GlfwWindowManager.hpp>

namespace Berserk {

    GlfwWindow::GlfwWindow(class GlfwWindowManager* manager, WindowDesc windowDesc) {
        BERSERK_ASSERT(manager);

        mManager = manager;
        mName = std::move(windowDesc.name);
        mTitle = std::move(windowDesc.title);
        mSize = windowDesc.size;

        mWindowHND = glfwCreateWindow(mSize.x(), mSize.y(), mTitle.GetStr(), nullptr, nullptr);

        glfwGetWindowPos(mWindowHND, &mPosition[0], &mPosition[1]);
        glfwGetWindowContentScale(mWindowHND, &mPixelRatio[0], &mPixelRatio[1]);
        glfwGetFramebufferSize(mWindowHND, &mFramebufferSize[0], &mFramebufferSize[1]);
        mIsInFocus = glfwGetWindowAttrib(mWindowHND, GLFW_FOCUSED);
        mVisibility = EWindowVisibility::Windowed;
    }

    GlfwWindow::~GlfwWindow() {
        if (mWindowHND) {
            glfwDestroyWindow(mWindowHND);
            mWindowHND = nullptr;
        }
    }

    void GlfwWindow::RequestFocus() {
        glfwRequestWindowAttention(mWindowHND);
    }

    void GlfwWindow::AddListener(WindowEventListener &listener) {
        BERSERK_ASSERT(!mListeners.Contains(&listener));
        mListeners.Add(&listener);
    }

    void GlfwWindow::RemoveListener(WindowEventListener &listener) {
        BERSERK_ASSERT(mListeners.Contains(&listener));
        mListeners.RemoveElement(&listener);
    }

    EWindowVisibility GlfwWindow::GetVisibility() const {
        return mVisibility;
    }

    Point2i GlfwWindow::GetPosition() const {
        return mPosition;
    }

    Size2i GlfwWindow::GetSize() const {
        return mSize;
    }

    Vec2f GlfwWindow::GetPixelRatio() const {
        return mPixelRatio;
    }

    bool GlfwWindow::IsInFocus() const {
        return mIsInFocus;
    }

    const String &GlfwWindow::GetName() const {
        return mName;
    }

    const String &GlfwWindow::GetTitle() const {
        return mTitle;
    }

    void GlfwWindow::OnFramebufferSizeChanged(const Size2i &newFramebufferSize) {
        mFramebufferSize = newFramebufferSize;

        WindowEvent event = GetEventBase(EWindowEventType::Framebuffer);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnWindowMoved(const Point2i &newPosition) {
        mPosition = newPosition;

        WindowEvent event = GetEventBase(EWindowEventType::Move);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnWindowResized(const Size2i &newSize) {
        mSize = newSize;

        WindowEvent event = GetEventBase(EWindowEventType::Resize);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnContentScaleChanged(const Vec2f &newScale) {
        mPixelRatio = newScale;

        WindowEvent event = GetEventBase(EWindowEventType::PixelRatio);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnFocusChanged(bool isInFocus) {
        mIsInFocus = isInFocus;

        WindowEvent event = GetEventBase(EWindowEventType::Focus);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnMinimized(bool minimized) {
        mVisibility = minimized? EWindowVisibility::Minimized: EWindowVisibility::Windowed;

        WindowEvent event = GetEventBase(EWindowEventType::Visibility);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    void GlfwWindow::OnClose() {
        glfwSetWindowShouldClose(mWindowHND, GLFW_FALSE);

        WindowEvent event = GetEventBase(EWindowEventType::Close);
        for (auto listener: mListeners)
            listener->OnWindowEvent(event);
    }

    WindowEvent GlfwWindow::GetEventBase(EWindowEventType type) const {
        WindowEvent event;
        event.eventType = type;
        event.size = mSize;
        event.position = mPosition;
        event.visibility = mVisibility;
        event.framebufferSize = mFramebufferSize;
        event.pixelRatio = mPixelRatio;
        event.focus = mIsInFocus;
        return event;
    }

}
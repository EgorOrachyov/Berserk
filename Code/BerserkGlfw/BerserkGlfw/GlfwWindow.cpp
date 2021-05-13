/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkGlfw/GlfwWindow.hpp>
#include <BerserkGlfw/GlfwWindowManager.hpp>

namespace Berserk {

    GlfwWindow::GlfwWindow(const Desc &desc) {
        mName = desc.name;
        mTitle = desc.title;
        mSize = desc.size;

        #ifdef BERSERK_TARGET_MACOS
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        #endif

        mHandle = glfwCreateWindow(mSize.x(), mSize.y(), mTitle.GetStr_C(), nullptr, nullptr);

        glfwGetWindowPos(mHandle, &mPosition[0], &mPosition[1]);
        glfwGetWindowContentScale(mHandle, &mPixelRatio[0], &mPixelRatio[1]);
        glfwGetFramebufferSize(mHandle, &mFramebufferSize[0], &mFramebufferSize[1]);

        // todo: basic format conversion
        if (!desc.icon.IsEmpty() && desc.icon.GetPixelFormat() == Image::PixelFormat::R8G8B8A8) {
            auto& icon = desc.icon;

            GLFWimage image{};
            image.width = icon.GetWidth();
            image.height = icon.GetHeight();
            image.pixels = (unsigned char*) icon.GetBufferRef()->GetData();

            glfwSetWindowIcon(mHandle, 1, &image);
        }

        mIsInFocus = glfwGetWindowAttrib(mHandle, GLFW_FOCUSED);
        OnWindowEvent = Event<const EventData&>(mEvent);

        glfwMakeContextCurrent(mHandle);
    }

    GlfwWindow::~GlfwWindow() {
        BERSERK_ASSERT(mHandle == nullptr);
    }

    void GlfwWindow::Close() {
        Guard<SpinMutex> guard(mMutex);

        if (mHandle && !mIsPendingRelease) {
            mIsPendingRelease = true;
            mSwapBuffers = false;
            GlfwWindowManager::Get().QueueWindowToRelease(mHandle);
        }
    }

    Math::Point2i GlfwWindow::GetPosition() const {
        Guard<SpinMutex> guard(mMutex);
        return mPosition;
    }

    Math::Size2i GlfwWindow::GetSize() const {
        Guard<SpinMutex> guard(mMutex);
        return mSize;
    }

    Math::Size2i GlfwWindow::GetFramebufferSize() const {
        Guard<SpinMutex> guard(mMutex);
        return mFramebufferSize;
    }

    Math::Vec2f GlfwWindow::GetPixelRatio() const {
        Guard<SpinMutex> guard(mMutex);
        return mPixelRatio;
    }

    bool GlfwWindow::IsInFocus() const {
        Guard<SpinMutex> guard(mMutex);
        return mIsInFocus;
    }

    bool GlfwWindow::IsClosed() const {
        Guard<SpinMutex> guard(mMutex);
        return mHandle == nullptr;
    }

    StringName GlfwWindow::GetName() const {
        Guard<SpinMutex> guard(mMutex);
        return mName;
    }

    String GlfwWindow::GetTitle() const {
        Guard<SpinMutex> guard(mMutex);
        return mTitle;
    }

    void* GlfwWindow::GetNativeHnd() const {
        return mHandle;
    }

    bool GlfwWindow::CanSwapBuffers() const {
        return mSwapBuffers;
    }

    void GlfwWindow::SwapBuffers() {
        assert(mHandle);
        glfwSwapBuffers(mHandle);
    }

    void GlfwWindow::ReleaseNativeHandler() {
        Guard<SpinMutex> guard(mMutex);

        BERSERK_ASSERT(mHandle && "Must be called once");
        glfwDestroyWindow(mHandle);
        mHandle = nullptr;
    }

    GLFWwindow* GlfwWindow::GetNativeHandle() const {
        return mHandle;
    }

    void GlfwWindow::OnFramebufferSizeChanged(const Math::Size2i &newFramebufferSize) {
        {
            Guard<SpinMutex> guard(mMutex);
            mFramebufferSize = newFramebufferSize;
        }

        EventData event = GetEventBaseData(EventType::FramebufferResized);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnWindowMoved(const Math::Point2i &newPosition) {
        {
            Guard<SpinMutex> guard(mMutex);
            mPosition = newPosition;
        }

        EventData event = GetEventBaseData(EventType::Moved);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnWindowResized(const Math::Size2i &newSize) {
        {
            Guard<SpinMutex> guard(mMutex);
            mSize = newSize;
        }

        EventData event = GetEventBaseData(EventType::Resized);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnContentScaleChanged(const Math::Vec2f &newScale) {
        {
            Guard<SpinMutex> guard(mMutex);
            mPixelRatio = newScale;
        }

        EventData event = GetEventBaseData(EventType::PixelRatioChanged);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnFocusChanged(bool isInFocus) {
        {
            Guard<SpinMutex> guard(mMutex);
            mIsInFocus = isInFocus;
        }

        EventData event = GetEventBaseData(mIsInFocus? EventType::FocusReceived: EventType::FocusLost);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnMinimized(bool minimized) {
        EventData event = GetEventBaseData(minimized? EventType::Minimized: EventType::Restored);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnMaximized(bool maximized) {
        EventData event = GetEventBaseData(maximized? EventType::Maximized: EventType::Restored);
        mEvent.Dispatch(event);
    }

    void GlfwWindow::OnClose() {
        EventData event = GetEventBaseData(EventType::CloseRequested);
        mEvent.Dispatch(event);
    }

    Window::EventData GlfwWindow::GetEventBaseData(EventType eventType) const {
        Guard<SpinMutex> guard(mMutex);

        EventData data;
        data.eventType = eventType;
        data.position = mPosition;
        data.windowSize = mSize;
        data.framebufferSize = mFramebufferSize;
        data.pixelRatio = mPixelRatio;
        data.focus = mIsInFocus;
        return data;
    }
}
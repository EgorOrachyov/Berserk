//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_GLFWWINDOW_H
#define BERSERK_GLFWWINDOW_H

#include <Application/IWindow.h>
#include <Strings/String.h>
#include <GLFW/glfw3.h>

namespace Berserk
{

    class ENGINE_API GlfwWindow final : public IWindow
    {
    public:

        GlfwWindow(uint32 width, uint32 height, const String& name);

        ~GlfwWindow() override;

        void setPosition(uint32 x, uint32 y) override;

        void setSize(uint32 width, uint32 height) override;

        void maximize() override;

        void minimize() override;

        void focus() override;

        void makeActiveRenderingTarget() override;

        uint32 getWidth() const override;

        uint32 getHeight() const override;

        uint32 getWidth_FrameBuffer() const override;

        uint32 getHeight_FrameBuffer() const override;

        uint32 getPosX() const override;

        uint32 getPosY() const override;

        bool isMovable() const override;

        bool isResizable() const override;

        bool isSizeChanged() const override;

        bool isPositionChanged() const override;

        const String &getName() const override;

    protected:

        friend class GlfwWindowManager;

        /** GLFW internal [called by manager]: updates window position, size and framebuffer size */
        void update();

    private:

        GLFWwindow* mWindowHandler = nullptr;

        uint32 mPosX;
        uint32 mPosY;
        uint32 mWidth;
        uint32 mHeight;
        uint32 mWidth_Framebuffer;
        uint32 mHeight_Framebuffer;

        bool mIsMovable = true;
        bool mIsResizable = false;
        bool mIsSizeChanged = false;
        bool mIsPosChanged = false;

        String mName;

    };

} // namespace Berserk

#endif //BERSERK_GLFWWINDOW_H
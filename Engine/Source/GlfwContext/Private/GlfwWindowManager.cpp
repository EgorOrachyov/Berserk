//
// Created by Egor Orachyov on 2019-07-17.
//

#include "GlfwWindowManager.h"
#include <Logging/DebugLogMacros.h>
#include <IO/OutputDevice.h>

namespace Berserk
{
    GlfwWindowManager::GlfwWindowManager(Berserk::IAllocator &allocator)
         : mAllocator(allocator),
           mWindowsList(allocator)
    {
        {
            // Setup glfw - Window and Input driver provider
            // In current implementation glfw will be initialized in the
            // Open GL Render device driver class

            if (!glfwInit())
            {
                DEBUG_LOG_WARNING("GlfwWindowManager: cannot initialize GLFW library");
                return;
            }

#ifdef PLATFORM_MAC
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        }
    }

    GlfwWindowManager::~GlfwWindowManager()
    {
        DEBUG_LOG_DISPLAY("GlfwWindowManager: destroy");
        glfwTerminate();
    }

    WindowRef GlfwWindowManager::createWindow(uint32 width, uint32 height, const String &name)
    {
        CriticalSection section(mMutex);
        CriticalSection glfwSection(mGLFWAccessMutex);

        TSharedPtr<IWindow>* ref = _findWindow(name);
        if (ref != nullptr)
        {
            DEBUG_LOG_WARNING("GlfwWindowManager: attempt to create window with duplicated name [name: %s]", name.get());
            return WindowRef();
        }

        auto _window = mAllocator.engine_new<GlfwWindow>(
                width,
                height,
                name,
                mGLFWAccessMutex);

        TSharedPtr<IWindow> window = TSharedPtr<IWindow>(_window, &mAllocator);
        mWindowsList.add(window);

        return WindowRef(window);
    }

    WindowRef GlfwWindowManager::findWindow(const String &name)
    {
        CriticalSection section(mMutex);

        for (auto window = mWindowsList.begin(); window != nullptr; window = mWindowsList.next())
        {
            if (window->get().getName() == name)
                return *window;
        }

        return WindowRef();
    }

    void GlfwWindowManager::update()
    {
        CriticalSection section(mMutex);
        CriticalSection glfwSection(mGLFWAccessMutex);

        auto iterator = mWindowsList.createRemoveIterator();

        for (auto ptr = iterator.begin(); ptr != nullptr; ptr = iterator.next())
        {
            auto window = dynamic_cast<GlfwWindow*>(ptr->pointer());
            window->update();

            if (window->shouldClose())
                iterator.removeCurrent();
        }

        glfwPollEvents();
    }

    TSharedPtr<IWindow>* GlfwWindowManager::_findWindow(const String &name) const
    {
        for (auto window = mWindowsList.begin(); window != nullptr; window = mWindowsList.next())
        {
            if (window->get().getName() == name)
                return window;
        }

        return nullptr;
    }

} // namespace Berserk
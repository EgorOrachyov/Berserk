//
// Created by Egor Orachyov on 2019-07-17.
//

#include "GlfwWindowManager.h"
#include <Logging/DebugLogMacros.h>

namespace Berserk
{
    GlfwWindowManager::GlfwWindowManager(Berserk::IAllocator &allocator)
        : mMapPool(WindowMap::getNodeSize(), PoolAllocator::DEFAULT_CHUNK_COUNT, allocator),
          mWindowMap(allocator, mMapPool),
          mAllocator(allocator)
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
        glfwTerminate();
    }

    IWindowRef GlfwWindowManager::createWindow(uint32 width, uint32 height, const String &name)
    {
        CriticalSection section(mMutex);

        IWindowRef* ref = mWindowMap.get(name);
        if (ref != nullptr)
        {
            DEBUG_LOG_WARNING("GlfwWindowManager: attempt to create window with duplicated name [name: %s]", name.get());
            return IWindowRef();
        }

        IWindowRef window = IWindowRef(mAllocator.engnie_new<GlfwWindow>(width, height, name), &mAllocator);
        mWindowMap.put(name, window);
        return window;
    }

    IWindowRef GlfwWindowManager::findWindow(const String &name)
    {
        CriticalSection section(mMutex);

        IWindowRef* ref = mWindowMap.get(name);
        if (ref != nullptr)
        {
            return *ref;
        }

        return IWindowRef();
    }

    void GlfwWindowManager::update()
    {
        CriticalSection section(mMutex);

        glfwPollEvents();

        for (auto pair = mWindowMap.begin(); pair != nullptr; pair = mWindowMap.next())
        {
            auto window = (GlfwWindow*) pair->value()->operator->();

            if (window->mShouldClose)
            {
                mRemoveList.add(window);
            }
            else
            {
                window->update();
            }
        }

        for (uint32 i = 0; i < mRemoveList.getSize(); i++)
        {
            mWindowMap.remove(mRemoveList.get(i)->mName);
        }

        mRemoveList.clearNoDestructorCall();
    }

} // namespace Berserk
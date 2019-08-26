//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_GLFWWINDOWMANAGER_H
#define BERSERK_GLFWWINDOWMANAGER_H

#include <Application/IWindowManager.h>
#include <Containers/THashMap.h>
#include <GlfwWindow.h>

namespace Berserk
{

    class ENGINE_API GlfwWindowManager final : public IWindowManager
    {
    public:

        GENERATE_NEW_DELETE(GlfwWindowManager);

        /**
         * Creates window manager based on glfw lib
         * @param allocator Used to allocate all the window resources
         */
        explicit GlfwWindowManager(IAllocator& allocator = Allocator::get());

        ~GlfwWindowManager() override;

        /** @copydoc IWindowManager::createWindow() */
        WindowRef createWindow(uint32 width, uint32 height, const String &name) override;

        /** @copydoc IWindowManager::findWindow() */
        WindowRef findWindow(const String &name) override;

        /** @copydoc IWindowManager::update() */
        void update() override;

    private:

        /** Internally finds window via its name */
        TSharedPtr<IWindow>* _findWindow(const String& name) const;

    private:

        typedef TArray<TSharedPtr<IWindow>> WindowsList;

        /** For internal usage */
        IAllocator& mAllocator;

        /** All the created windows by manager */
        WindowsList mWindowsList;

        /** Thread-safe access for UI thread ? */
        Mutex mMutex;

        /** Mutex to access thread-unsafe glfw functions */
        Mutex mGLFWAccessMutex;

    };

} // namespace Berserk

#endif //BERSERK_GLFWWINDOWMANAGER_H
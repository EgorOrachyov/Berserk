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

        GlfwWindowManager(IAllocator& allocator = Allocator::get());

        ~GlfwWindowManager() override;

        IWindowRef createWindow(uint32 width, uint32 height, const String &name) override;

        IWindowRef findWindow(const String &name) override;

        void update() override;

    private:

        typedef THashMap<String, TSharedPtr<IWindow>> WindowMap;

        /** Pool for map allocations */
        PoolAllocator mMapPool;

        /** All the created windows by manager */
        WindowMap mWindowMap;

        /** Tmp list for windows to remove */
        TArray<GlfwWindow*> mRemoveList;

        /** Thread-safe access for UI thread ? */
        Mutex mMutex;

        /** Mutex to access thread-unsafe glfw functions */
        Mutex mGLFWAccessMutex;

        /** For internal usage */
        IAllocator& mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_GLFWWINDOWMANAGER_H
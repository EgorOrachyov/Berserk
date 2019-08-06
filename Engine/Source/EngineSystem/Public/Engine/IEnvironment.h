//
// Created by Egor Orachyov on 2019-08-02.
//

#ifndef BERSERK_IENVIRONMENT_H
#define BERSERK_IENVIRONMENT_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Console/IConsoleManager.h>
#include <Logging/ILogManager.h>
#include <Application/IWindowManager.h>
#include <Config/IConfigTable.h>
#include <Resource/IResourceManager.h>
#include <Object/IObjectManager.h>

namespace Berserk
{

    /**
     * Engine environment provides unified access to all major engine systems.
     * Environment exists all time of the engine execution. All the objects, provided by the
     * environment will be destroyed only after all the game data is freed and the application is
     * going to be shut down.
     *
     * @note Thread-Safe
     */
    class ENGINE_API IEnvironment
    {
    public:

        /** @return Primary engine main config table with engine global settings */
        virtual IConfigTable& getConfigTable() const = 0;

        /** @return Window manager of the engine */
        virtual IWindowManager& getWindowManager() const = 0;

        /** @return Engine global object manager [handles all the registered game objects] */
        virtual IObjectManager& getObjectManager() const = 0;

        /** @return Engine primary log manager for logged output */
        virtual ILogManager& getLogManager() const = 0;

        /** @return Engine output device [for debug purposes] */
        virtual IOutputDevice& getOutputDevice() const = 0;

        /** @return Engine main cosole manager with console commands and variables */
        virtual IConsoleManager& getConsoleManager() const = 0;

        /** @return Engine resource manager for main and pool threads resources */
        virtual IResourceManager& getResourceManager() const = 0;

        /** @return Frame allocator for single frame allocations */
        virtual IAllocator& getFrameAllocator() const = 0;

        /** @return Double frame allocator for allocations, available in current and next frame */
        virtual IAllocator& getDoubleFrameAllocator() const = 0;

        /** @return General purpose engine allocator for tmp/long-living allocations */
        virtual IAllocator& getAllocator() const = 0;

        /** @return OS target platform allocator (malloc wrapper) */
        virtual IAllocator& getAllocator_OS() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_IENVIRONMENT_H
//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_IWINDOWMANAGER_H
#define BERSERK_IWINDOWMANAGER_H

#include <HAL/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Application/IWindow.h>

namespace Berserk
{

    /** Manages all the created windows in the application */
    class ENGINE_API IWindowManager
    {
    public:

        virtual ~IWindowManager() = default;

        /**
         * Creates window with default properties and specified params
         * @param width Window width [in units]
         * @param height Window height [in units]
         * @param name Window name and viewed caption/title of the window
         * @return Created window reference
         */
        virtual WindowRef createWindow(uint32 width, uint32 height, const String& name) = 0;

        /**
         * Finds window via its ame
         * @param name Name/title of the window
         * @return Window ref or null-ref if it is not found
         */
        virtual WindowRef findWindow(const String& name) = 0;

        /**
         * Updates manager and all the created windows
         * Removes closed windows, updates callbacks, params, windows' properties.
         */
        virtual void update() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IWINDOWMANAGER_H
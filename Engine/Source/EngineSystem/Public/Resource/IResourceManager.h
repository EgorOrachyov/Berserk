//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_IRESOURCEMANAGER_H
#define BERSERK_IRESOURCEMANAGER_H

#include <Resource/ResourceHandle.h>
#include <Rendering/Font.h>

namespace Berserk
{

    /**
     * Engine resource manager interface, provides unified I/O resource access
     * for all engine sub-systems. Provides:
     * 1) Resource loading, handling and memory allocation/free
     * 2) Async resource loading
     * 3) Caching resource for future reuse
     * 4) Run-time resource reload without updating clients
     *
     * @note Resources, loaded by manager should be used only by main and pool threads
     * @note Thread-safe, resources could be requested from any thread [main or pool]
     * @note Resource manager will exist all time of the application execution till it is shut down
     */
    class ENGINE_API IResourceManager
    {
    public:

        virtual ResourceHandle<Font> loadFont(const char* name) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCEMANAGER_H
//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_IRESOURCECACHE_H
#define BERSERK_IRESOURCECACHE_H

#include <Resource/ResourceHandle.h>
#include <Rendering/Font.h>
#include <Rendering/Image.h>

namespace Berserk
{

    /**
     * Engine resource cache interface, provides unified I/O resource access
     * for all engine sub-systems. Features:
     * 1) Resource loading, handling and memory allocation/free
     * 2) Async resource loading
     * 3) Caching resource for future reuse
     * 4) Run-time resource reload without updating clients
     *
     * @note Resources, loaded by manager should be used only by main and pool threads
     * @note Thread-safe, resources could be requested from any thread [main or pool]
     * @note Resource cache will exist all time of the application execution till it is shut down
     */
    class ENGINE_API IResourceCache
    {
    public:

        /** Set available memory cache size to cache loaded resource */
        virtual void setCacheSize(uint64 sizeInBytes) const = 0;

        /** @return Cache size in bytes */
        virtual uint64 getCacheSize() const = 0;

        /** @return Current memory usage by cache in bytes */
        virtual uint64 getCacheUsage() const = 0;

        /**
         * Loads font from TTF file and returns handle to ready to use font data
         * @param name Resource name to find it later in the resource cache
         * @param path Full os file path and file name to the font
         * @param cache Set this field in true to cache this resource in manager
         * @return Ready to use font resource handle
         */
        virtual ResourceHandle<Font> loadFont(const char* name, const char* path, bool cache = true) = 0;

        /**
         * Loads font from png/bmp/jpg file and returns handle to ready to use image data
         * @param name Resource  name to find it later in the resource cache
         * @param path Full os file path and file name to the resource
         * @param cache Set this field in true to cache this resource in manager
         * @return Ready to use font resource handle
         */
        virtual ResourceHandle<Image> loadImageData(const char* name, const char* path, bool cache = true) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCECACHE_H
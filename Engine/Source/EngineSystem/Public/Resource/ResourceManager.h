//
// Created by Egor Orachyov on 2019-08-04.
//

#ifndef BERSERK_RESOURCEMANAGER_H
#define BERSERK_RESOURCEMANAGER_H

#include <Resource/IResourceManager.h>
#include <Memory/PoolAllocator.h>
#include <Importers/IFontImporter.h>
#include <Importers/IImageImporter.h>

namespace Berserk
{

    /** Default engine resource manager implementation */
    class ENGINE_API ResourceManager final : public IResourceManager, public Allocatable
    {
    public:

        void setCacheSize(uint64 sizeInBytes) const override;

        uint64 getCacheSize() const override;

        uint64 getCacheUsage() const override;

        ResourceHandle<Font> loadFont(const char *name, const char *path, bool cache) override;

        ResourceHandle<Image> loadImageData(const char *name, const char *path, bool cache) override;

    private:

        /**
         * Internally finds resource as loaded in cache or in async list load
         * @param name Resource name to find
         * @return Resource handler or null handler
         */
        ResourceHandle<IResource> find(const char* name);

    private:

        /** Default caching limit for loaded resources */
        static const uint64 DEFAULT_CACHE_SIZE = MiB * 32;

        volatile uint64 mCacheSize;
        volatile uint64 mCacheUsage;

        /** Allocator for internal usage */
        IAllocator& mAllocator;

        /** Allocator used to allocate all resources and its data */
        IAllocator& mResAllocator;

        /** Allocate res handle data memory */
        PoolAllocator mResHandleDataPool;

        /** To import TTF fonts */
        IFontImporter& mFontImporter;

        /** To import png/bmp/jpg images */
        IImageImporter& mImageImporter;

    };

} // namespace Berserk

#endif //BERSERK_RESOURCEMANAGER_H
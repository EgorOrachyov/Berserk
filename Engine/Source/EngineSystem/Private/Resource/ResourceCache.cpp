//
// Created by Egor Orachyov on 2019-08-04.
//

#include "Resource/ResourceCache.h"

namespace Berserk
{

    void ResourceCache::setCacheSize(uint64 sizeInBytes) const
    {

    }

    uint64 ResourceCache::getCacheSize() const
    {
        return 0;
    }

    uint64 ResourceCache::getCacheUsage() const
    {
        return 0;
    }

    ResourceHandle<Font> ResourceCache::loadFont(const char *name, const char *path, bool cache)
    {
    }

    ResourceHandle<Image> ResourceCache::loadImageData(const char *name, const char *path, bool cache)
    {
    }
} // namespace Berserk

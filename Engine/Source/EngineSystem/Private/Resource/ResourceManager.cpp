//
// Created by Egor Orachyov on 2019-08-04.
//

#include "Resource/ResourceManager.h"

namespace Berserk
{

    void ResourceManager::setCacheSize(uint64 sizeInBytes) const
    {

    }

    uint64 ResourceManager::getCacheSize() const
    {
        return 0;
    }

    uint64 ResourceManager::getCacheUsage() const
    {
        return 0;
    }

    ResourceHandle<Font> ResourceManager::loadFont(const char *name, const char *path, bool cache)
    {
    }

    ResourceHandle<Image> ResourceManager::loadImageData(const char *name, const char *path, bool cache)
    {
    }
} // namespace Berserk

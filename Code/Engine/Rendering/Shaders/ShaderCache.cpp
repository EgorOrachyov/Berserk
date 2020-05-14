/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Shaders/ShaderCache.h>
#include <Platform/ISystem.h>
#include <IO/ArchiveFile.h>

namespace Berserk {
    namespace Rendering {

        const char CACHE_HEADER[] = "Shaders/Cache/ShaderCacheEntries.meta";

        ShaderCache::ShaderCache(CString prefixPath) {

            mPrefixPath = std::move(prefixPath);
            auto cacheFile = mPrefixPath + CACHE_HEADER;

            auto& system = ISystem::getSingleton();
            auto file = system.openFile(cacheFile, EFileMode::Read);

            if (file.isNotNull() && file->isOpen()) {
                // Has file and it is open (try to load)
                ArchiveFile archiveFile(file);
                archiveFile >> mLastCached;
                archiveFile >> mCacheEntries;
            }
            else {
                // Creates an empty cache entries file
                updateCacheEntries();
            }
        }

        ShaderCache::~ShaderCache() {
            if (mUpdateCacheOnClose) updateCacheEntries();
        }
        
        bool ShaderCache::containsShader(const CString &name) const {
            return mCacheEntries.contains(name);
        }

        TRef<const Shader> ShaderCache::load(const CString &name) {

        }

        void ShaderCache::updateCacheEntries() {
            auto& system = ISystem::getSingleton();
            auto time = system.getTime();
            char buffer[64];
            sprintf(buffer, "%u.%u.%u %u:%u:%u", time.year, time.month+1, time.dayMonth+1, time.hour, time.min, time.sec);

            auto cacheFile = mPrefixPath + CACHE_HEADER;
            auto file = system.openFile(cacheFile, EFileMode::Write);

            BERSERK_COND_ERROR_RET(file.isNotNull() && file->isOpen(), "Failed to create cache file");

            mLastCached = buffer;

            ArchiveFile archiveFile(file);
            archiveFile << mLastCached;
            archiveFile << mCacheEntries;
        }

    }
}
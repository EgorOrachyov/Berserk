/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERCACHE_H
#define BERSERK_SHADERCACHE_H

#include <Rendering/Resources/Shader.h>
#include <Containers/TSet.h>

namespace Berserk {
    namespace Rendering {


        /**
         * @brief Shader Cache
         * Creates, loads and updates shaders cache.
         */
        class ShaderCache final {
        public:

            /**
             * Opens shader cache at specified prefix path to the engine directory.
             * If cache was not created, creates new one.
             * If cache is present, reads all cache entries names.
             * @param prefixPath Path to the engine directory
             */
            ShaderCache(CString prefixPath);
            ~ShaderCache();

            /** Pass true to force cache update before the engine is closed */
            void setUpdateOnClose(bool update);

            /**
             * Updates shader cache. Writes new entries to the cache.
             * @note Potentially long since require write all shaders to the disk.
             */
            void updateCacheEntries();

            /**
             * Attempts to add specified shader to the cache. If shader with specified
             * name already cached, then its cache data will be updated.
             * @param shader Initialized and fully created shader object
             * @return True if shader successfully cached
             */
            bool cacheShader(const Shader& shader);

            /**
             * Attempts to find shader in cache
             * @param name Shader look-up defined global name
             * @return True if shader in the cache
             */
            bool containsShader(const CString& name) const;

            /**
             * Attempts to find shader from cache, and load from disc.
             * @param name Shader look-up defined global name
             * @param[out] shader Loaded from the cache shader
             * @return True if successfully load and compile shader
             */
            bool loadFromCache(const CString &name, Shader &shader);

            /**
             * Reads cached shaders entries names (global shaders names)
             * @param[out] entries Array to store shaders names
             */
            void getCachedShadersNames(TArray<CString> &entries) const;

            /** @return Prefix path to the engine shaders sources and cache */
            const CString &getPrefixPath() const { return mPrefixPath; }

        protected:

            /** Internal - create string from time */
            void getCurrentDateString(const ISystem::Time &time, CString &string);

            struct CachedData {
                uint32 index = 0;
                uint32 size = 0;
                CString lastUpdated;
                CString fileName;

                friend Archive& operator<<(Archive& archive, const CachedData& data) {
                    archive << data.index;
                    archive << data.size;
                    archive << data.lastUpdated;
                    archive << data.fileName;
                    return archive;
                }

                friend Archive& operator>>(Archive& archive, CachedData& data) {
                    archive >> data.index;
                    archive >> data.size;
                    archive >> data.lastUpdated;
                    archive >> data.fileName;
                    return archive;
                }
            };

            /** Tre if must update cache when destroy this cache */
            bool mUpdateCacheOnClose = false;
            /** Prefix path to the engine shaders sources and cache */
            CString mPrefixPath;
            /** Last time cache was updated */
            CString mLastCached;
            /** Cached shaders names */
            TMap<CString,CachedData> mCacheEntries;
        };


    }
}


#endif //BERSERK_SHADERCACHE_H

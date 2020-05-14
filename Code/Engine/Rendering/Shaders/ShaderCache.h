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
             * @param prefixPath
             */
            ShaderCache(CString prefixPath);
            ~ShaderCache();

            /** Updates shader cache. Writes new entries to the cache. */
            void updateCacheEntries();

            /**
             * Attempts to find shader in cache
             * @param name Shader look-up defined global name
             * @return True if shader in the cache
             */
            bool containsShader(const CString& name) const;

            /**
             * Attempts to find shader from cache, and load from disc.
             * @param name Shader look-up defined global name
             * @return Compiled shader reference (or null pointer if failed to load shader)
             */
            TRef<const Shader> load(const CString& name);

            /** @return Prefix path to the engine shaders sources and cache */
            const CString &getPrefixPath() const { return mPrefixPath; }

            /** @return Cached shaders names */
            const TSet<CString> &getCachedShadersNames() const { return mCacheEntries ; }

        protected:
            /** Tre if must update cache when destroy this cache */
            bool mUpdateCacheOnClose = false;
            /** Prefix path to the engine shaders sources and cache */
            CString mPrefixPath;
            /** Last time cache was updated */
            CString mLastCached;
            /** Cached shaders names */
            TSet<CString> mCacheEntries;
        };


    }
}


#endif //BERSERK_SHADERCACHE_H

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROGRAMCACHE_H
#define BERSERK_SHADERPROGRAMCACHE_H

#include <Paths.h>
#include <ShaderCore/ShaderProgram.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Compiled programs cache
         *
         * Caches shader programs by its name with additional meta information
         * in order to reuse compiled code and share RHI resource among several
         * shaders with different input params.
         */
        class ShaderProgramCache {
        public:

            ShaderProgramCache();
            ~ShaderProgramCache() = default;

            /** @return True if cache already contains shader */
            bool containsProgram(const CString& name) const;

            /** @return True if successfully preload program by its relative name */
            bool preloadProgram(const CString& relativeFilePath, EPathType pathType);

            /** @return Program handle if successfully found program by its unique name, otherwise null */
            TPtrShared<ShaderProgram> find(const CString& name) const;

            /** @return Program handle if successfully load program by its relative name, otherwise null */
            TPtrShared<ShaderProgram> load(const CString& relativeFilePath, EPathType pathType);

            /** Debug feature: print cache content to standard output */
            void showEntriesInfo() const;

            /** @return Global program cache instance */
            static ShaderProgramCache& getSingleton();

        private:

            TPtrShared <ShaderProgram> loadInternal(const CString &relativeFilePath, EPathType pathType);

            struct CacheEntry {
                TPtrShared<ShaderProgram> program;
                CString loadPath;
                TimeValue loadTime;
                mutable TimeValue accessTime;
            };

            TMap<CString,CacheEntry> mCache;

            static ShaderProgramCache* gProgramCache;
        };

    }
}

#endif //BERSERK_SHADERPROGRAMCACHE_H

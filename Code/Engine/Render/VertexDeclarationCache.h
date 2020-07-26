/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXDECLARATIONCACHE_H
#define BERSERK_VERTEXDECLARATIONCACHE_H

#include <RenderResources/VertexDeclaration.h>


namespace Berserk {
    namespace Render {

        /** Caches declarations by name in runtime for reuse */
        class VertexDeclarationCache {
        public:

            VertexDeclarationCache();
            ~VertexDeclarationCache();

            /** Caches specified declaration at runtime */
            void cache(const TPtrShared<VertexDeclaration>& declaration);

            /** @return True if contains declaration with specified name */
            bool contains(const CString& declarationName) const;

            /** @return Declaration if found by name */
            TPtrShared<VertexDeclaration> find(const CString& declarationName);

            /** @return Engine cache singleton */
            static VertexDeclarationCache& getSingleton();

        private:

            struct CacheEntry {
                TPtrShared<VertexDeclaration> declaration;
                TimeValue cached;
            };

            TMap<CString,CacheEntry> mCache;

            static VertexDeclarationCache* gCache;
        };

    }
}

#endif //BERSERK_VERTEXDECLARATIONCACHE_H
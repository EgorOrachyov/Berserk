/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <VertexDeclarationCache.h>

namespace Berserk {
    namespace Render {

        VertexDeclarationCache* VertexDeclarationCache::gCache = nullptr;

        VertexDeclarationCache::VertexDeclarationCache() {
            if (gCache) {
                fprintf(stderr, "[BERSERK Render] Allowed only single VertexDeclarationCache instance");
                return;
            }

            gCache = this;
        }

        VertexDeclarationCache::~VertexDeclarationCache() {
            gCache = nullptr;
        }

        void VertexDeclarationCache::cache(const TPtrShared<VertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET(declaration.isNotNull(), "Attempt to cache null declaration");

            if (!contains(declaration->getName())) {
                auto& name = declaration->getName();
                auto& entry = mCache[name];
                entry.declaration = declaration;
                entry.cached = TimeValue::nowAsTime();
            }
        }

        bool VertexDeclarationCache::contains(const CString &declarationName) const {
            return mCache.contains(declarationName);
        }

        TPtrShared<VertexDeclaration> VertexDeclarationCache::find(const CString &declarationName) {
            auto ref = mCache.getPtr(declarationName);
            return ref.isNotNull()? ref->declaration: nullptr;
        }

        VertexDeclarationCache& VertexDeclarationCache::getSingleton() {
            return *gCache;
        }

    }
}
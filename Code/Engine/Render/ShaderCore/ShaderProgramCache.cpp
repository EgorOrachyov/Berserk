/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderProgramCache.h>
#include <ShaderCore/ShaderProgramCompiler.h>

namespace Berserk {
    namespace Render {

        ShaderProgramCache* ShaderProgramCache::gProgramCache = nullptr;

        ShaderProgramCache::ShaderProgramCache() {
            if (gProgramCache) {
                fprintf(stderr, "[BERSERK Render] Only single ShaderProgramCache could be set as singleton");
                return;
            }

            gProgramCache = this;
        }

        bool ShaderProgramCache::containsProgram(const CString &name) const {
            return mCache.contains(name);
        }

        bool ShaderProgramCache::preloadProgram(const CString &relativeFilePath, EPathType pathType) {
            auto fullPath = Paths::getFullPathFor(relativeFilePath, pathType);

            for (auto& e: mCache) {
                auto& entry = e.second();

                if (entry.loadPath == fullPath)
                    return true;
            }

            return loadInternal(relativeFilePath, pathType).isNotNull();
        }

        TPtrShared<ShaderProgram> ShaderProgramCache::find(const CString &name) const {
            auto entry = mCache.getPtr(name);

            if (entry.isNotNull()) {
                entry->accessTime = TimeValue::nowAsTime();
                return entry->program;
            }

            return nullptr;
        }

        TPtrShared<ShaderProgram> ShaderProgramCache::load(const CString &relativeFilePath, EPathType pathType) {
            auto fullPath = Paths::getFullPathFor(relativeFilePath, pathType);

            for (auto& e: mCache) {
                auto& entry = e.second();

                if (entry.loadPath == fullPath) {
                    entry.accessTime = TimeValue::nowAsTime();
                    return entry.program;
                }
            }

            return loadInternal(relativeFilePath, pathType);
        }

        void ShaderProgramCache::showDebugInfo() const {
            auto& system = System::getSingleton();

            printf("ShaderProgramCache\n");

            for (auto& e: mCache) {
                auto& name = e.first();
                auto& entry = e.second();

                auto access = system.getTime(entry.accessTime).toString();
                auto load = system.getTime(entry.loadTime).toString();

                printf(" Entry: Name=%s,Loaded=%s,LastAccess=%s,LoadPath=%s\n",
                        name.data(), load.data(), access.data(), entry.loadPath.data());
            }
        }

        ShaderProgramCache& ShaderProgramCache::getSingleton() {
            return *gProgramCache;
        }

        TPtrShared<ShaderProgram> ShaderProgramCache::loadInternal(const CString &relativeFilePath, EPathType pathType) {
            ShaderProgramCompiler programCompiler(relativeFilePath, pathType);

            if (!programCompiler.canCompile()) {
                BERSERK_ERROR("Cannot compile shader program %s: %s", relativeFilePath.data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            programCompiler.compile();

            if (!programCompiler.isCompiled()) {
                BERSERK_ERROR("Failed to compile shader program %s: %s", relativeFilePath.data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            if (!programCompiler.canCreateProgram()) {
                BERSERK_ERROR("Cannot create shader program instance %s: %s", relativeFilePath.data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            auto& programFile = programCompiler.getShaderFile();
            auto program = programCompiler.createProgram();
            auto programName = programFile.getShaderName();

            CacheEntry programEntry;
            programEntry.program = program;
            programEntry.loadTime = programEntry.accessTime = TimeValue::nowAsTime();
            programEntry.loadPath = programFile.getFilePath();

            mCache.move(programName, programEntry);

            return program;
        }

    }
}
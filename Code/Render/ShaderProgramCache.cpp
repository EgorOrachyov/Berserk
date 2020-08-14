/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderProgramCache.h>
#include <ShaderProgramCompiler.h>

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

        TPtrShared<ShaderProgram> ShaderProgramCache::find(const CString &name) const {
            auto entry = mCache.getPtr(name);

            if (entry.isNotNull()) {
                entry->accessTime = TimeValue::nowAsTime();
                return entry->program;
            }

            return nullptr;
        }

        void ShaderProgramCache::cacheProgram(const Berserk::TPtrShared<Berserk::Render::ShaderProgram> &program) {
            BERSERK_COND_ERROR_RET(program.isNotNull(), "Passed null program");

            CString programName = program->getName();
            CacheEntry programEntry;
            programEntry.program = program;
            programEntry.loadTime = programEntry.accessTime = TimeValue::nowAsTime();


            mCache.move(programName, programEntry);
        }

        void ShaderProgramCache::showDebugInfo() const {
            auto& system = System::getSingleton();

            printf("ShaderProgramCache\n");

            for (auto& e: mCache) {
                auto& name = e.first();
                auto& entry = e.second();

                auto access = system.getTime(entry.accessTime).toString();
                auto load = system.getTime(entry.loadTime).toString();

                printf(" Entry: Name=%s,Loaded=%s,LastAccess=%s\n",
                        name.data(), load.data(), access.data());
            }
        }

        ShaderProgramCache& ShaderProgramCache::getSingleton() {
            return *gProgramCache;
        }

    }
}
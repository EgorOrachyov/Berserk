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
#include <Rendering/RenderModule.h>

namespace Berserk {
    namespace Rendering {

        ShaderCache::ShaderCache(CString prefixPath) {
            auto& config = RenderModule::getSingleton().getConfig();

            mPrefixPath = std::move(prefixPath);
            mPrefixPath += config.getShadersCachePath();
            auto cacheFile = mPrefixPath + config.getShaderCacheMetaName();

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

        void ShaderCache::setUpdateOnClose(bool update) {
            mUpdateCacheOnClose = update;
        }

        void ShaderCache::updateCacheEntries() {
            auto& system = ISystem::getSingleton();
            auto& config = RenderModule::getSingleton().getConfig();

            auto cacheFile = mPrefixPath + config.getShaderCacheMetaName();
            auto file = system.openFile(cacheFile, EFileMode::Write);

            BERSERK_COND_ERROR_RET(file.isNotNull() && file->isOpen(), "Failed to create cache file");

            auto time = system.getTime();
            getCurrentDateString(time, mLastCached);

            ArchiveFile archiveFile(file);
            archiveFile << mLastCached;
            archiveFile << mCacheEntries;
        }

        bool ShaderCache::cacheShader(const Shader &shader) {
            if (containsShader(shader.getName())) {

            }
            else {
                BERSERK_COND_ERROR_RET_VALUE(false, shader.isInitialized(), "An attempt to cache uninitialized shader");
                BERSERK_COND_ERROR_RET_VALUE(false, shader.supportsSerialization(), "Shader '%s' does not supports caching", shader.getName().data());

                CachedData cachedData;

                auto &system = ISystem::getSingleton();
                auto time = system.getTime();

                getCurrentDateString(time, cachedData.lastUpdated);
                cachedData.fileName = shader.getName() + ".cache";

                auto fullPath = mPrefixPath + cachedData.fileName;
                auto file = system.openFile(fullPath, EFileMode::Write);

                if (file.isNotNull() && file->isOpen()) {
                    ArchiveFile archive(file);
                    auto result = shader.serialize(archive);

                    if (result) {
                        cachedData.size = archive.getFile().getSize();
                        mCacheEntries.emplace(shader.getName(), std::move(cachedData));

                        return true;
                    }
                }
            }

            return false;
        }

        bool ShaderCache::containsShader(const CString &name) const {
            return mCacheEntries.contains(name);
        }

        bool ShaderCache::loadFromCache(const CString &name, Shader &shader) {
            auto data = mCacheEntries.getPtr(name);
            BERSERK_COND_ERROR_RET_VALUE(false, data.isNotNull(), "Cache does not contain shader '%s'", name.data())

            auto& fileName = data->fileName;
            auto fullPath = mPrefixPath + fileName;

            auto &system = ISystem::getSingleton();

            auto file = system.openFile(fullPath, EFileMode::Read);

            if (file.isNotNull() && file->isOpen()) {
                ArchiveFile archive(file);
                return shader.deserialize(archive);
            }

            return false;
        }

        void ShaderCache::getCachedShadersNames(Berserk::TArray<Berserk::CString> &entries) const {
            entries.ensureToAdd(mCacheEntries.size());
            for (const auto& entry: mCacheEntries) {
                entries.add(entry.first());
            }
        }

        void ShaderCache::getCurrentDateString(const ISystem::Time &time, CString &string) {
            char buffer[64];
            sprintf(buffer, "%u.%u.%u %u:%u:%u", time.year, time.month+1, time.dayMonth+1, time.hour, time.min, time.sec);
            string = buffer;
        }

    }
}
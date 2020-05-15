/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Shaders/ShaderManager.h>
#include <Rendering/RenderModule.h>
#include <Platform/ISystem.h>
#include <IO/Ini.h>

namespace Berserk {
    namespace Rendering {

        ShaderManager::ShaderManager(CString prefixPath) {
            auto& module = RenderModule::getSingleton();
            auto& config = module.getConfig();

            mDefaultCache = module.getShaderCache();
            mPrefixPath = std::move(prefixPath);
            mPrefixPath += config.getShadersPath();

            auto result = loadProfile(config.getShaderProfileName());

            if (!result) mShaderProfiles.emplace();
        }

        ShaderManager::~ShaderManager() {
            // do nothing
            // later: update shader cache if needed
        }

        bool ShaderManager::loadProfile(const CString &name) {
            auto& system = ISystem::getSingleton();
            auto& config = RenderModule::getSingleton().getConfig();

            auto fileName = mPrefixPath + name + ".ini";
            auto file = system.openFile(fileName, EFileMode::Read);

            BERSERK_COND_ERROR_RET_VALUE(false, file.isNotNull() && file->isOpen(), "Failed to open shader profile '%s'", name.data());
            Ini ini = *file;
            BERSERK_COND_ERROR_RET_VALUE(false, ini.isParsed(), "Failed to parse file profile '%s'", name.data());

            mShaderProfiles.emplace(name, config.getTargetDeviceType(), ini);
            return true;
        }

        TPtrShared<Shader> ShaderManager::findShader(const CString &name) const {
            auto find = mShaders.getPtr(name);
            return (find.isNotNull()? *find: nullptr);
        }

        TPtrShared<Shader> ShaderManager::loadShader(const CString &name) {
            auto find = mShaders.getPtr(name);

            if (find.isNotNull()) {
                return *find;
            }

            if (mDefaultCache->containsShader(name)) {
                // load from cache

            }

            TArrayStatic<TRef<const ShaderProfile::Entry>> sourcesNames;
            getDefaultProfile().findEntriesWithPrefix(name, sourcesNames);
            EShaderLanguage language = getDefaultProfile().getLanguage();

            BERSERK_COND_ERROR_RET_VALUE(nullptr, sourcesNames.size() > 0, "Filed to find shader '%s' sources in default profile", name.data())

            const char vertSuffix[] = ".vs";
            const char fragSuffix[] = ".fs";

            TRef<const CString> vertFileName;
            TRef<const CString> fragFileName;

            for (auto ref: sourcesNames) {
                if (ref->first().ends(vertSuffix))
                    vertFileName = ref->second();

                if (ref->first().ends(fragSuffix))
                    fragFileName = ref->second();
            }

            if (vertFileName.isNotNull() && fragFileName.isNotNull()) {
                TArray<uint8> sourceVert;
                TArray<uint8> sourceFrag;

                loadShaderSource(*vertFileName, sourceVert);
                loadShaderSource(*fragFileName, sourceFrag);

                auto shader = TPtrShared<Shader>::make(name, language, sourceVert, sourceFrag);

                if (shader->isInitialized()) {
                    mShaders.add(name, shader);
                    return shader;
                }

                BERSERK_ERROR("Filed to compile shader '%s'", name.data());
            }

            return nullptr;
        }

        void ShaderManager::loadShaderSource(const CString &path, TArray<uint8> &source) {
            auto& system = ISystem::getSingleton();
            auto fileName = mPrefixPath + path;

            auto file = system.openFile(fileName, EFileMode::Read);
            BERSERK_COND_ERROR_RET(file.isNotNull() && file->isOpen(), "Failed open file '%s'", path.data())

            auto size = file->getSize();
            source.resize(size);
            auto result = file->read(source.data(), size);
            BERSERK_COND_ERROR_RET(result == EError::OK, "Failed to read source data from file '%s'", path.data());
        }

    }
}
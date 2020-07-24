/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderProgramCompiler.h>
#include <String/CStringBuilder.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        ShaderProgramCompiler::ShaderProgramCompiler(const CString &relativePathToShader, EPathType pathType)
            : mShaderFile(relativePathToShader, pathType) {

            if (!mShaderFile.isFileParsed()) {
                mInfoMessage = "Shader file is not parsed";
                return;
            }

            if (mShaderFile.getShaderType() != EShaderFileType::Program) {
                mInfoMessage = "Shader file has invalid type [must be Program]";
                return;
            }

            auto& device = RHIDevice::getSingleton();
            auto name = device.getDeviceName();

            if (!mShaderFile.supportsDevice(name)) {
                mInfoMessage = CString{"Device '"} + name + "' is not supported by shader file";
                return;
            }

            auto types = mShaderFile.getShaderTypesForDevice(name);

            if (!types.contains(EShaderType::Vertex) || !types.contains(EShaderType::Fragment)) {
                mInfoMessage = "Incomplete set of source files";
            }

            mDeviceName = std::move(name);
            mCanCompile = true;
        }

        void ShaderProgramCompiler::compile() {
            // Algorithm of compilation:
            // 1. Check version string to append in the file beginning
            // 2. Append Engine global macro definitions (future)
            // 2. Check dependencies (1 dept recursion only)
            // 3. Get complete source code
            // 4. Create RHI handle
            // 5. Reflect meta data

            if (!canCompile())
                return;

            auto& device = RHIDevice::getSingleton();
            auto types = mShaderFile.getShaderTypesForDevice(mDeviceName);
            auto names = mShaderFile.getShaderNamesForDevice(mDeviceName);

            TArrayStatic<CStringBuilder> sources;
            sources.resize(names.size());

            CString versionString;
            CString versionStringPrefix = "#version "; // todo: support for different languages

            if (mShaderFile.isVersionSpecifiedForDevice(mDeviceName))
                versionString = mShaderFile.getVersionForDevice(mDeviceName);

            for (auto& source: sources) {
                appendVersion(source, versionStringPrefix, versionString);
            }

            ////////////////////////////////////////////////////////
            // Append Engine global macro definitions here (future)
            ////////////////////////////////////////////////////////

            auto& dependencies = mShaderFile.getDependencies();
            auto pathType = mShaderFile.getPathTypeOfShaderFile();

            for (auto& dependencyName: dependencies) {
                ShaderFile dependency(dependencyName, pathType);

                if (!dependency.isFileParsed()) {
                    mInfoMessage = CString{"Failed load dependency "} + dependencyName;
                    return;
                }

                auto dependencyType = dependency.getShaderType();
                auto sourceType = ShaderFile::getShaderTypeForDependency(dependencyType);

                if (types.contains(sourceType)) {
                    if (!appendDependency(findDataForType(sourceType, types, sources), dependency)) return;
                    continue;
                }

                mInfoMessage = CString{"Dependency has inappropriate type "} + dependencyName;
                return;
            }

            for (uint32 i = 0; i < types.size(); i++) {
                auto type = types[i];
                if (!appendDataFromFile(findDataForType(type, types, sources), names[i], pathType)) return;
            }

#if 1
            auto vs = findDataForType(EShaderType::Vertex, types, sources).toString();
            auto fs = findDataForType(EShaderType::Fragment, types, sources).toString();

            printf("=============== Vertex Shader: ===============\n%s\n"
                   "=============== Fragment Shader: ===============\n%s\n", vs.data(), fs.data());
#endif

            mCachedSources.resize(sources.size());
            for (uint32 i = 0; i < sources.size(); i++) {
                auto& entry = mCachedSources[i];
                entry.first() = types[i];
                entry.second().add((uint8*) sources[i].data(), sources[i].length());
            }

            RHIShaderViewDesc desc;
            desc.resize(mCachedSources.size());

            for (uint32 i = 0; i < desc.size(); i++) {
                desc[i].type = mCachedSources[i].first();
                desc[i].code = &mCachedSources[i].second().internal();
            }

            auto language = mShaderFile.getLanguageForDevice(mDeviceName);

            mShader = device.createShader(language, desc);
            mShaderCreated = mShader.isNotNull();

            if (!mShaderCreated) {
                mInfoMessage = "Failed to create RHI shader resource";
                return;
            }

            mMetaData = device.createShaderMeta(mShader);
            mMetaDataCreated = mMetaData.isNotNull();

            if (!mMetaDataCreated) {
                mInfoMessage = "Failed to create RHI shader meta data resource";
                return;
            }

            mCompiled = true;
        }

        bool ShaderProgramCompiler::canCompile() const {
            return mCanCompile;
        }

        bool ShaderProgramCompiler::canCreateProgram() const {
            return mShaderCreated && mMetaDataCreated;
        }

        const CString& ShaderProgramCompiler::getInfoMessage() const {
            return mInfoMessage;
        }

        TPtrShared<ShaderProgram> ShaderProgramCompiler::createProgram() {
            if (!isCompiled())
                return nullptr;

            if (!canCreateProgram())
                return nullptr;

            if (mProgramCreated)
                return nullptr;


            mProgramCreated = true;

            return TPtrShared<ShaderProgram>::make(mShaderFile.getShaderName(), mShader, mMetaData, mCachedSources);
        }
        
        void ShaderProgramCompiler::appendVersion(CStringBuilder &data, const CString &prefix, const CString &version) {
            data.append(prefix);
            data.append(version);
            data.append("\n");
        }

        bool ShaderProgramCompiler::appendDependency(struct CStringBuilder &data, ShaderFile &dependency) {
            auto dependencyType = dependency.getShaderType();
            auto types = dependency.getShaderTypesForDevice(mDeviceName);
            auto names = dependency.getShaderNamesForDevice(mDeviceName);

            if (dependencyType == EShaderFileType::VertexShaderInclude) {
                uint32 index = -1;
                if (types.getIndexOf(EShaderType::Vertex, index)) {
                    return appendDataFromFile(data, names[index], dependency.getPathTypeOfShaderFile());
                }

                return returnError(CString{"No Vertex shader provided for "} + mDeviceName);
            }

            if (dependencyType == EShaderFileType::FragmentShaderInclude) {
                uint32 index = -1;
                if (types.getIndexOf(EShaderType::Fragment, index)) {
                    return appendDataFromFile(data, names[index], dependency.getPathTypeOfShaderFile());
                }

                return returnError(CString{"No Fragment shader provided for "} + mDeviceName);
            }

            return returnError(CString{"Invalid type for dependency "} + dependency.getShaderName());
        }
        
        bool ShaderProgramCompiler::appendDataFromFile(struct CStringBuilder &data, const CString &relativePath, EPathType pathType) {
            auto fullPath = Paths::getFullPathFor(relativePath, pathType);
            auto file = System::getSingleton().openFile(fullPath, EFileMode::Read);

            if (file.isNotNull() && file->isOpen()) {
                uint32 size = file->getSize();
                TArray<char> buffer;
                buffer.resize(size);

                auto result = file->read(buffer.data(), size * sizeof(char));

                if (result == EError::OK) {
                    data.append(buffer.data(), size);
                    return true;
                }

                return returnError(CString{"Failed to read data from file "} + fullPath);
            }

            return returnError(CString{"Failed to open source file "} + fullPath);
        }

        CStringBuilder& ShaderProgramCompiler::findDataForType(EShaderType type, const TArrayStatic<EShaderType> &types, TArrayStatic<CStringBuilder> &data) {
            uint32 index;
            types.getIndexOf(type, index);
            return data[index];
        }

    }
}
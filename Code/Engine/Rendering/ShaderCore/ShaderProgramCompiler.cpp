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

            for (auto type: types) {
                auto& p = mCachedSources.emplace();
                p.first() = type;
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

            CString versionString;
            CString versionStringPrefix = "#version "; // todo: support for different languages

            if (mShaderFile.isVersionSpecifiedForDevice(mDeviceName))
                versionString = mShaderFile.getVersionForDevice(mDeviceName);

            CStringBuilder vertexShaderSourceCode;
            CStringBuilder fragmentShaderSourceCode;

            appendVersion(vertexShaderSourceCode, versionStringPrefix, versionString);
            appendVersion(fragmentShaderSourceCode, versionStringPrefix, versionString);

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

                if (dependencyType == EShaderFileType::VertexShaderInclude) {
                    auto result = appendDependency(vertexShaderSourceCode, dependency);
                    if (!result) return;
                    continue;
                }

                if (dependencyType == EShaderFileType::FragmentShaderInclude) {
                    auto result = appendDependency(fragmentShaderSourceCode, dependency);
                    if (!result) return;
                    continue;
                }

                mInfoMessage = CString{"Dependency has inappropriate type "} + dependencyName;
                return;
            }

            ////

            auto types = mShaderFile.getShaderTypesForDevice(mDeviceName);
            auto names = mShaderFile.getShaderNamesForDevice(mDeviceName);

            for (uint32 i = 0; i < types.size(); i++) {
                auto type = types[i];

                if (type == EShaderType::Vertex) {
                    auto result = appendDataFromFile(vertexShaderSourceCode, names[i], pathType);
                    if (!result) return;
                    continue;
                }

                if (type == EShaderType::Fragment) {
                    auto result = appendDataFromFile(fragmentShaderSourceCode, names[i], pathType);
                    if (!result) return;
                    continue;
                }

                mInfoMessage = CString{"Shader source has unsupported type"};
                return;
            }

            auto vs = vertexShaderSourceCode.toString();
            auto fs = fragmentShaderSourceCode.toString();

            printf("%s\n%s\n", vs.data(), fs.data());

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

        TPtrShared<ShaderProgram> ShaderProgramCompiler::create() const {
            return nullptr;
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
        
    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderProgramCompiler.h>
#include <String/CStringBuilder.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        ShaderProgramCompiler::ShaderProgramCompiler(CString shaderName, ShaderFile& shaderFile, const ShaderCompilerInsertions& insertionsGlsl)
            : mShaderName(std::move(shaderName)), mShaderFile(shaderFile), mInsertions(insertionsGlsl) {

            if (!mShaderFile.isFileParsed()) {
                mInfoMessage = "Shader file is not parsed";
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
            // 1. Append Engine global macro definitions (future)
            // 2. Check include dependencies (1 dept recursion only)
            // 3. Get complete source code
            // 4. Create RHI handle
            // 5. Reflect meta data

            if (!canCompile())
                return;


            auto& device = RHIDevice::getSingleton();
            auto types = mShaderFile.getShaderTypesForDevice(mDeviceName);
            auto names = mShaderFile.getShaderNamesForDevice(mDeviceName);

            mCachedSources.resize(types.size());

            ////////////////////////////////////////////////////////
            // Append Engine global macro definitions here (future)
            ////////////////////////////////////////////////////////

            auto pathType = mShaderFile.getPathTypeOfShaderFile();
            auto includes = mShaderFile.getIncludePathsForDevice(mDeviceName);

            for (uint32 i = 0; i < types.size(); i++) {
                auto type = types[i];
                auto& name = names[i];
                auto fullPath = Paths::getFullPathFor(name, pathType);
                auto file = System::getSingleton().openFile(fullPath, EFileMode::Read);

                if (file.isNotNull() && file->isOpen()) {
                    uint32 size = file->getSize();
                    TArray<char> buffer;
                    buffer.resize(size);

                    auto result = file->read(buffer.data(), size);

                    if (result == EError::OK) {
                        ProcessorInsertionsGlsl insertionsGlsl{};

                        switch (type) {
                            case EShaderType::Vertex:
                                insertionsGlsl = mInsertions.vertex;
                                break;
                            case EShaderType::Fragment:
                                insertionsGlsl = mInsertions.fragment;
                                break;
                            default:
                                break;
                        };

                        ShaderProcessorGlsl processor(buffer, includes, pathType, insertionsGlsl);
                        processor.process();

                        if (!processor.isProcessed()) {
                            returnError(CString{"Failed to process include for file "} + fullPath + " :" + processor.getErrorMessage());
                            return;
                        }

                        auto& builder = processor.getSourceBuilder();
                        auto& entry = mCachedSources[i];
                        entry.first() = type;
                        entry.second().add((const uint8*) builder.data(), builder.length());
                        continue;
                    }

                    returnError(CString{"Failed to read data from file "} + fullPath);
                    return;
                }

                returnError(CString{"Failed to open file "} + fullPath);
                return;
            }

#if 0
            auto& vsData = findDataForType(EShaderType::Vertex);
            auto& fsData = findDataForType(EShaderType::Fragment);
            CString vs((char*)vsData.data(), vsData.size());
            CString fs((char*)fsData.data(), fsData.size());

            printf("=============== Vertex Shader: ===============\n%s\n"
                   "=============== Fragment Shader: ===============\n%s\n", vs.data(), fs.data());
#endif

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

            return TPtrShared<ShaderProgram>::make(mShaderName, mShader, mMetaData, mCachedSources);
        }
        
        BinaryData& ShaderProgramCompiler::findDataForType(EShaderType type) {
            for (auto& e: mCachedSources) {
                if (e.first() == type) {
                    return e.second();
                }
            }

            BERSERK_ERROR_FAIL("No such entry");
        }

    }
}
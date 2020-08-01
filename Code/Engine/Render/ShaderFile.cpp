/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderFile.h>

namespace Berserk {
    namespace Render {

        ShaderFile::ShaderFile(const CString &relativePathToFile, EPathType pathType) {
            mPathType = pathType;
            mFilePath = Paths::getFullPathFor(relativePathToFile, pathType);
            auto& system = System::getSingleton();
            auto file = system.openFile(mFilePath, EFileMode::Read);

            BERSERK_COND_ERROR_RET(file.isNotNull() && file->isOpen(), "Failed to open file '%s'", mFilePath.data());

            JsonDocument document = *file;

            BERSERK_COND_ERROR_RET(document.isParsed(), "Failed to parse file '%s'", mFilePath.data());

            parse(document.getContent());
            mFileParsed = true;
        }

        void ShaderFile::parse(JsonValue &value) {
            auto& body = value.getObject();
            {
                mName = std::move(body["Name"].getString());
                mDescription = std::move(body["Description"].getString());
                mAuthor = std::move(body["Author"].getString());
                mVersion = std::move(body["Version"].getString());
                mCreated = std::move(body["Created"].getString());
            }

            auto& platforms = body["Platforms"].getObject();
            {
                for (auto& p: platforms)
                    mPerPlatformInfo.move(p.first().getString(), p.second());
            }
        }
        
        bool ShaderFile::supportsDevice(const CString &deviceName) {
            return mPerPlatformInfo.contains(deviceName);
        }

        TArrayStatic<EShaderType> ShaderFile::getShaderTypesForDevice(const CString &deviceName) {
            if (!supportsDevice(deviceName))
                return {};

            TArrayStatic<EShaderType> types;
            auto& device = mPerPlatformInfo[deviceName].getObject();
            auto knownTypesNames = RHIDefinitionsUtil::getShaderTypesAsString();

            for (auto& e: device) {
                if (knownTypesNames.contains(e.first().getString())) {
                    types.add(RHIDefinitionsUtil::getShaderTypeFromString(e.first().getString()));
                }
            }

            return types;
        }
        
        TArrayStatic<CString> ShaderFile::getShaderNamesForDevice(const CString &deviceName) {
            if (!supportsDevice(deviceName))
                return {};

            TArrayStatic<CString> names;
            auto& device = mPerPlatformInfo[deviceName].getObject();
            auto knownTypesNames = RHIDefinitionsUtil::getShaderTypesAsString();

            for (auto& e: device) {
                if (knownTypesNames.contains(e.first().getString())) {
                    names.add(e.second().getString());
                }
            }

            return names;
        }

        TArrayStatic<CString> ShaderFile::getIncludePathsForDevice(const Berserk::CString &deviceName) {
            if (!supportsDevice(deviceName))
                return {};

            TArrayStatic<CString> includePaths;
            auto& device = mPerPlatformInfo[deviceName].getObject();

            if (device.contains("Includes")) {
                auto& includes = device["Includes"].getArray();
                for (auto& path: includes) {
                    includePaths.add(path.getString());
                }
            }

            return includePaths;
        }

        EShaderLanguage ShaderFile::getLanguageForDevice(const Berserk::CString &deviceName) {
            if (!supportsDevice(deviceName))
                return EShaderLanguage::Undefined;

            return RHIDefinitionsUtil::getLanguageFromString(mPerPlatformInfo[deviceName].getObject()["Language"].getString());
        }

    }
}
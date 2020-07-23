/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderFile.h>

namespace Berserk {
    namespace Rendering {

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
                mFileType = getShaderFileTypeFromString(body["Type"].getString().data());
            }

            auto& dependencies = body["Dependencies"].getArray();
            {
                mDependencies.ensureToAdd(dependencies.size());
                for (auto& d: dependencies)
                    mDependencies.add(d.getString());
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

        bool ShaderFile::isVersionSpecifiedForDevice(const CString &deviceName) {
            return mPerPlatformInfo.contains(deviceName) && mPerPlatformInfo[deviceName].getObject().contains("Version");
        }
        
        CString ShaderFile::getVersionForDevice(const CString &deviceName) {
            return isVersionSpecifiedForDevice(deviceName) ? mPerPlatformInfo[deviceName].getObject()["Version"] : "";
        }
        
        TArrayStatic<EShaderType> ShaderFile::getShaderTypesForDevice(const CString &deviceName) {
            if (!supportsDevice(deviceName))
                return {};

            TArrayStatic<EShaderType> types;
            auto& device = mPerPlatformInfo[deviceName].getObject();

            if (device.contains("Vertex"))
                types.add(EShaderType::Vertex);
            if (device.contains("Fragment"))
                types.add(EShaderType::Fragment);

            return types;
        }
        
        TArrayStatic<CString> ShaderFile::getShaderNamesForDevice(const CString &deviceName) {
            if (!supportsDevice(deviceName))
                return {};

            TArrayStatic<CString> names;
            auto& device = mPerPlatformInfo[deviceName].getObject();

            if (device.contains("Vertex"))
                names.add(device["Vertex"].getString());
            if (device.contains("Fragment"))
                names.add(device["Fragment"].getString());

            return names;
        }

        EShaderFileType ShaderFile::getShaderFileTypeFromString(const char *string) {
            if (CStringUtility::compare(string, "Program") == 0)
                return EShaderFileType::Program;
            if (CStringUtility::compare(string, "VertexShaderInclude") == 0)
                return EShaderFileType::VertexShaderInclude;
            if (CStringUtility::compare(string, "FragmentShaderInclude") == 0)
                return EShaderFileType::FragmentShaderInclude;
            return EShaderFileType::Unknown;
        }

        const char* ShaderFile::getShaderFileTypeStringFromEnum(Rendering::EShaderFileType type) {
            switch (type) {
                case EShaderFileType::Program:
                    return "Program";
                case EShaderFileType::FragmentShaderInclude:
                    return "FragmentShaderInclude";
                case EShaderFileType::VertexShaderInclude:
                    return "VertexShaderInclude";
                default:
                    return "Unknown";
            }
        }
    }
}
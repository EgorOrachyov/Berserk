/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERFILE_H
#define BERSERK_SHADERFILE_H

#include <Paths.h>
#include <IO/JsonDocument.h>
#include <Platform/System.h>
#include <RHI/RHIDefinitions.h>
#include <Containers/BinaryData.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {
    namespace Rendering {

        enum class EShaderFileType {
            /** Is a full shader program that could be compiled */
            Program,
            /** Code of such dependency could be included in the vertex shader */
            VertexShaderInclude,
            /** Code of such dependency could be included in the fragment shader */
            FragmentShaderInclude,
            /** Type was not specified by user */
            Unknown
        };

        /**
         * @brief Wrapper for shader meta data
         *
         * C++ side access for shaders code json human readable meta data.
         * Opens shaders json meta, parses and converts to runtime accessible values.
         */
        class ShaderFile {
        public:

            /**
             * Open shader meta data from disk
             * @param relativePathToFile Relative path to the shader
             */
            ShaderFile(const CString &relativePathToFile, EPathType pathType);
            ~ShaderFile() = default;

            /** @return Shader unique usable name */
            const CString& getShaderName() const { return mName; }

            /** @return Shader optional text description  */
            const CString& getShaderDescription() const { return mDescription; }

            /** @return Author info */
            const CString& getAuthorName() const { return mAuthor; }

            /** @return When was created */
            const CString& getWhenCreated() const { return mCreated; }

            /** @return String optional file version */
            const CString& getStringVersion() const { return mVersion; }

            /** @return Dependencies of the shader file */
            const TArray<CString> &getDependencies() const { return mDependencies; }

            /** @return True if file was successfully parsed */
            bool isFileParsed() const { return mFileParsed; }

            /** @return True if has sources for this platform */
            bool supportsDevice(const CString& deviceName);

            /** @return True if version of sources specififed for this platform */
            bool isVersionSpecifiedForDevice(const CString &deviceName);

            /** @return Version string for this platform */
            CString getVersionForDevice(const CString& deviceName);

            /** @return Types of shaders for device */
            TArrayStatic<EShaderType> getShaderTypesForDevice(const CString& deviceName);

            /** @return Path of shaders for device (same order as for types) */
            TArrayStatic<CString> getShaderNamesForDevice(const CString& deviceName);

            /** @return Shader language specified for device sources */
            EShaderLanguage getLanguageForDevice(const CString& deviceName);

            /** @return Type of the shader file */
            EShaderFileType getShaderType() const { return mFileType; }

            /** @return Path type of the loaded shader file */
            EPathType getPathTypeOfShaderFile() const { return mPathType; }

            /** @return Path used to load this file */
            const CString& getFilePath() const { return mFilePath; }

            static bool isDependencyFor(EShaderFileType type, EShaderType shader);
            static EShaderType getShaderTypeForDependency(EShaderFileType type);
            static EShaderFileType getShaderFileTypeFromString(const char* string);
            static const char* getShaderFileTypeStringFromEnum(EShaderFileType type);

        private:

            void parse(JsonValue& value);

            EPathType mPathType;
            CString mFilePath;
            CString mName;
            CString mDescription;
            CString mAuthor;
            CString mVersion;
            CString mCreated;
            TArray<CString> mDependencies;
            EShaderFileType mFileType = EShaderFileType::Unknown;
            bool mFileParsed = false;
            TMap<CString,Variant> mPerPlatformInfo;
        };

    }
}



#endif //BERSERK_SHADERFILE_H

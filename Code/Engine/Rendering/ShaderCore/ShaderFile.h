/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERFILE_H
#define BERSERK_SHADERFILE_H

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
            /** Only code fragment which could be included in Program */
            Fragment,
            /** Type was not specified by user */
            Unknown
        };

        enum class EDependencyType {
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
             * @param shader Relative path to the shader
             */
            ShaderFile(const CString& shader);
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

            /** @return Types of shaders for device */
            TArrayStatic<EShaderType> getShaderTypesForDevice(const CString& deviceName);

            /** @return Path of shaders for device (same order as for types) */
            TArrayStatic<CString> getShaderNamesForDevice(const CString& deviceName);

            /** @return Type of the shader file */
            EShaderFileType getShaderType() const { return mFileType; }

            /** @return Type of the shader file dependency (only for dependencies) */
            EDependencyType getDependencyType() const { return mDependencyType; }

            /** @return Path used to load this file */
            const CString& getFilePath() const { return mFilePath; }

            static EShaderFileType getShaderFileTypeFromString(const char* string);
            static EDependencyType getDependencyTypeFromString(const char* string);

        private:

            void parse(JsonValue& value);

            CString mFilePath;
            CString mName;
            CString mDescription;
            CString mAuthor;
            CString mVersion;
            CString mCreated;
            TArray<CString> mDependencies;
            EShaderFileType mFileType = EShaderFileType::Unknown;
            EDependencyType mDependencyType = EDependencyType::Unknown;
            bool mFileParsed = false;
            TMap<CString,Variant> mPerPlatformInfo;
        };

    }
}



#endif //BERSERK_SHADERFILE_H

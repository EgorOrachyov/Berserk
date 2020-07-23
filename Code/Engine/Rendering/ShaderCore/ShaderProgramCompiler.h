/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROGRAMCOMPILER_H
#define BERSERK_SHADERPROGRAMCOMPILER_H

#include <ShaderCore/ShaderFile.h>
#include <ShaderCore/ShaderProgram.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @breif Program compiler
         *
         * Builds shader sources from source code and dependencies,
         * adds macro definitions, and compiles (creates RHI) for current RHI platform.
         */
        class ShaderProgramCompiler {
        public:

            /**
             * Accepts relative shader path and type of this path to compile shader
             * @param relativePathToShader Relative engine path
             * @param pathType Path of the type relation
             *
             * @note To compile shader program you must call compile method
             */
            ShaderProgramCompiler(const CString &relativePathToShader, EPathType pathType);

            /** Attempts to compile shader */
            void compile();

            /** @return True if shader RHI is created */
            bool isShaderCreated() const { return mShaderCreated; }

            /** @return True if meta data RHI is created */
            bool isMetaDataCreated() const { return mMetaDataCreated; }

            /** @return True if shader compiled */
            bool isCompiled() const { return mCompiled; }

            /** @return True if sources can be compiled (has code and appropriate type) */
            bool canCompile() const;

            /** @return True if can create shader program */
            bool canCreateProgram() const;

            /** @return Some useful info if something went wrong */
            const CString& getInfoMessage() const;

            /** @return Creates shader program from compiled result (null if fails) */
            TPtrShared<ShaderProgram> create() const;

        private:

            void appendVersion(class CStringBuilder& data, const CString& prefix, const CString& version);
            bool appendDependency(class CStringBuilder &data, ShaderFile &dependency);
            bool appendDataFromFile(struct CStringBuilder &data, const CString &relativePath, EPathType pathType);
            bool returnError(CString message) { mInfoMessage = std::move(message); return false; }

            bool mCanCompile = false;
            bool mCompiled = false;
            bool mShaderCreated = false;
            bool mMetaDataCreated = false;
            CString mInfoMessage;
            CString mDeviceName;
            ShaderFile mShaderFile;
            TPtrShared<RHIShader> mShader;
            TPtrShared<RHIShaderMetaData> mMetaData;
            TArrayStatic<TPair<EShaderType,BinaryData>> mCachedSources;

        };

    }
}

#endif //BERSERK_SHADERPROGRAMCOMPILER_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROGRAMCOMPILER_H
#define BERSERK_SHADERPROGRAMCOMPILER_H

#include <ShaderFile.h>
#include <ShaderProgram.h>
#include <ShaderProcessorGlsl.h>

namespace Berserk {
    namespace Render {

        /**
         * @breif Program compiler
         *
         * Builds shader sources from source code and dependencies,
         * adds macro definitions, and compiles (creates RHI) for current RHI platform.
         */
        class ShaderProgramCompiler {
        public:

            /**
             * Create shader compiler from the shader
             * @param shaderName Name assigned to the shader
             * @param shaderFile File with shader data
             * @param insertionsGlsl Data to be explicitly inserted in the shader source code
             * @note To compile shader program you must call compile method
             */
            ShaderProgramCompiler(CString shaderName, ShaderFile& shaderFile, const ShaderInsertionsGlsl& insertionsGlsl = ShaderInsertionsGlsl());

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

            /** @return Program shader file */
            const ShaderFile& getShaderFile() const { return mShaderFile; }

            /** @return Creates shader program from compiled result (null if fails) */
            TPtrShared<ShaderProgram> createProgram();

        private:

            bool returnError(CString message) { mInfoMessage = std::move(message); return false; }
            BinaryData& findDataForType(EShaderType type);

            bool mProgramCreated = false;
            bool mCanCompile = false;
            bool mCompiled = false;
            bool mShaderCreated = false;
            bool mMetaDataCreated = false;
            CString mInfoMessage;
            CString mDeviceName;
            CString mShaderName;
            ShaderFile& mShaderFile;
            ShaderInsertionsGlsl mInsertions;
            TPtrShared<RHIShader> mShader;
            TPtrShared<RHIShaderMetaData> mMetaData;
            TArrayStatic<TPair<EShaderType,BinaryData>> mCachedSources;

        };

    }
}

#endif //BERSERK_SHADERPROGRAMCOMPILER_H
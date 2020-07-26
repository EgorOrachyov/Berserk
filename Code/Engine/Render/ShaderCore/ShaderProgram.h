/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROGRAM_H
#define BERSERK_SHADERPROGRAM_H

#include <RHI/RHIResources.h>
#include <RHI/RHIShaderMetaData.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Shader program
         *
         * Complete compiled shader program with its RHI resource handle,
         * meta data and source code, which could be cached and shared among
         * different shaders with different set of input uniform attributes.
         */
        class ShaderProgram {
        public:

            /** Accept compiled sources (no null ptr - assert check) */
            ShaderProgram(CString name, TPtrShared<RHIShader> handle, TPtrShared<RHIShaderMetaData> meta,  TArrayStatic<TPair<EShaderType,BinaryData>> &sources);
            ~ShaderProgram() = default;

            /** @return Unique name for caching and look-up */
            const CString& getName() const { return mProgramName; }

            /** @return Compiled shader handle */
            const TPtrShared<RHIShader> &getShader() const { return mShader; }

            /** @return Meta data (reflection info) */
            const TPtrShared<RHIShaderMetaData> &getMetaData() const { return mMetaData; }

            /** @return Source code per stages */
            const TArrayStatic<TPair<EShaderType,BinaryData>> &getCachedSources() const { return mCachedSources; }

        private:
            /** Name for look-up */
            CString mProgramName;
            TPtrShared<RHIShader> mShader;
            TPtrShared<RHIShaderMetaData> mMetaData;
            TArrayStatic<TPair<EShaderType,BinaryData>> mCachedSources;
        };

    }
}

#endif //BERSERK_SHADERPROGRAM_H
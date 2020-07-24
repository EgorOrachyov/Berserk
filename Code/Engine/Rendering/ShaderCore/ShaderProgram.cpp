/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderProgram.h>

namespace Berserk {
    namespace Render {

        ShaderProgram::ShaderProgram(CString name, TPtrShared<RHIShader> handle,
                                     TPtrShared<RHIShaderMetaData> meta,
                                     TArrayStatic<TPair<EShaderType, BinaryData>> &sources) {

            BERSERK_COND_ERROR_RET(handle.isNotNull(), "Shader handle must be valid");
            BERSERK_COND_ERROR_RET(meta.isNotNull(), "Meta data must be valid");

            mProgramName = std::move(name);
            mShader = std::move(handle);
            mMetaData = std::move(meta);
            mCachedSources = std::move(sources);
        }

    }
}
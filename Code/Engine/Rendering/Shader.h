/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADER_H
#define BERSERK_SHADER_H

#include <RHI/RHIResources.h>
#include <RHI/RHIShaderMetaData.h>

namespace Berserk {

    /**
     * @brief Shader program
     *
     * Compiled shader program and its params info for the rendering thread
     */
    class Shader {
    public:

    private:
        CString mName;
        TPtrShared<RHIShader> mShaderHandle;
        TPtrShared<RHIShaderMetaData> mMetaData;
    };

}

#endif //BERSERK_SHADER_H
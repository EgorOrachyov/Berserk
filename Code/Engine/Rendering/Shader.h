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
#include <Rendering/VertexPolicy.h>

namespace Berserk {

    /**
     * @brief Shader program
     *
     * Compiled shader program and its params info for the rendering thread
     */
    class Shader {
    public:

        Shader(CString name, EShaderLanguage language, const TArray<uint8> &vertex, const TArray<uint8> &fragment);
        ~Shader() = default;

        /** @return True if RHI resources were successfully created */
        bool isValid() const;

        /** @return Shader param by name if present */
        TRef<const ShaderParam> findParam(const CString& name) const { return mMetaData->findParam(name); }

        /** @return Shader uniform block by name if present */
        TRef<const ShaderUniformBlock> findUniformBlock(const CString& name) const { return mMetaData->findUniformBlock(name); }

        const CString &getName() const { return mName; }
        const EShaderLanguage &getLanguage() const { return mLanguage; }
        const TPtrShared<RHIShader> &getShaderHandle() const { return mShaderHandle; }
        const TPtrShared<RHIShaderMetaData> &getMetaData() const { return mMetaData; }

    private:
        CString mName;
        EShaderLanguage mLanguage;
        TPtrShared<RHIShader> mShaderHandle;
        TPtrShared<RHIShaderMetaData> mMetaData;
    };

}

#endif //BERSERK_SHADER_H
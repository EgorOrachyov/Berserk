/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderCore/ShaderProgram.h>
#include <RenderResources/VertexDeclaration.h>

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

        bool ShaderProgram::isDeclarationCompatible(const struct VertexDeclaration &declaration) {
            auto& elements = mMetaData->getVertexShaderAttributes();
            auto& toCheck = declaration.getElements();

            if (elements.size() != toCheck.size())
                return false;

            for (auto& e: elements) {
                bool found = false;

                for (auto& c: toCheck) {
                    if (e.getLocation() == c.location && e.getDataType() == c.type) {
                        found = true;
                        break;
                    }
                }

                if (!found)
                    return false;
            }

            return true;
        }

    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ContextUniformData.h>
#include <ShaderCore/Shader.h>
#include <ShaderCore/ShaderProgramCache.h>

namespace Berserk {
    namespace Render {

        bool Shader::isUsable() const {
            return mProgram.isNotNull() &&
                   mUniformData.isNotNull() &&
                   mPipeline.isNotNull() &&
                   mDeclaration.isNotNull();
        }

        void Shader::use(RHIDrawList &drawList) {
            BERSERK_COND_ERROR_RET(isUsable(), "Shader is not created");

            if (mUniformData->isDirty())
                mUniformData->updateDataGPU();

            mPipeline->bind(drawList);
            mUniformData->bind(drawList);

            mTimeLastUsed = TimeValue::nowAsTime();
        }

        void Shader::setName(Berserk::CString name) {
            mName = std::move(name);
        }

        bool Shader::initializeProgram(const CString &pathToShader, EPathType pathType) {
            auto& cache = ShaderProgramCache::getSingleton();

            mProgram = cache.load(pathToShader, pathType);
            return mProgram.isNotNull();
        }

        bool Shader::initializeUniformData(ContextUniformData& context) {
            auto& globals = context.getBuffersNames();
            auto& meta = mProgram->getMetaData();
            mUniformData = TPtrShared<ShaderUniformBindings>::make(meta);

            for (auto& name: globals) {
                if (meta->hasUniformBlock(name)) {
                    mUniformData->associateUniformBuffer(name, context.findBufferForBlock(name));
                }
            }

            mUniformData->associateUniformBuffers();
            return true;
        }

    }
}
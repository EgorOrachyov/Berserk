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

        void Shader::use(RHIDrawList &drawList) {
            BERSERK_COND_ERROR_RET(canUse(), "Shader is not created");

            if (mUniformData->isDirty())
                mUniformData->updateDataGPU();

            auto& uniformSetRHI = mUniformData->getRHI();

            // todo: drawList.bindPipeline();
            drawList.bindUniformSet(uniformSetRHI);

            mTimeLastUsed = TimeValue::nowAsTime();
        }

        void Shader::setName(Berserk::CString name) {
            mName = std::move(name);
        }

        bool Shader::initializeProgram(const CString &pathToShader, EPathType pathType) {
            if (!canUse())
                return false;

            auto& cache = ShaderProgramCache::getSingleton();

            mProgram = cache.load(pathToShader, pathType);
            mIsUsable = mProgram.isNotNull();

            return mIsUsable;
        }

        bool Shader::initializeUniformData(ContextUniformData& context) {
            if (!canUse())
                return false;

            auto& globals = context.getBuffersNames();
            auto& meta = mProgram->getMetaData();
            mUniformData = TPtrShared<ShaderUniformBindings>::make(meta);

            for (auto& name: globals) {
                if (meta->hasUniformBlock(name)) {
                    mUniformData->associateUniformBuffer(name, context.findBufferForBlock(name));
                }
            }

            mUniformData->associateUniformBuffers();

            return mIsUsable;
        }

        bool Shader::canUse() {
            return mIsUsable;
        }

    }
}
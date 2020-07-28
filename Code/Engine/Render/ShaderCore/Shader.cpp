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

        Shader::Shader(CString name, TPtrShared<Render::ShaderProgram> program, TPtrShared<VertexDeclaration> declaration, RHIGraphicsPipelineState pipelineState)
            : mName(std::move(name)),
              mTimeLastUsed(TimeValue::nowAsTime()),
              mProgram(std::move(program)),
              mDeclaration(std::move(declaration)),
              mPipelineState(std::move(pipelineState)) {
        }

        void Shader::use(RHIDrawList &drawList) {
            drawList.bindPipeline(mPipelineState);
            mTimeLastUsed = TimeValue::nowAsTime();
        }

    }
}
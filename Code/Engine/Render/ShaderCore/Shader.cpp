/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ContextUniformData.h>
#include <ShaderCore/Shader.h>
#include <ShaderCore/ShaderBindings.h>
#include <ShaderCore/ShaderProgramCache.h>

namespace Berserk {
    namespace Render {

        Shader::Shader(CString name, TPtrShared<Render::ShaderProgram> program, TPtrShared<VertexDeclaration> declaration)
            : mName(std::move(name)),
              mProgram(std::move(program)),
              mDeclaration(std::move(declaration)) {
        }

        TPtrShared<ShaderBindings> Shader::allocateBindings() const {
            return TPtrShared<ShaderBindings>::make(mProgram->getMetaData());
        }

    }
}
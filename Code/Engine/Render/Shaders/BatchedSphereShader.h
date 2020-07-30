/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BATCHEDSPHERESHADER_H
#define BERSERK_BATCHEDSPHERESHADER_H

#include <ShaderCore/ShaderFactory.h>
#include <ShaderCore/ShaderProgramCache.h>
#include <RenderResources/VertexDeclarationCache.h>
#include <RenderResources/VertexDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        class BatchedSphereShader : public ShaderFactory {
        public:

            BatchedSphereShader() : ShaderFactory("BatchedSphereShader") {

            }

            TPtrShared<Shader> create() override {
                if (mCached.isNotNull())
                    return mCached;

                const CString programPath = "Engine/Shaders/BatchedSphereShader.json";
                const CString declarationName = "inPos|inWorldPos.inColor.inRadius";

                auto& programCache = ShaderProgramCache::getSingleton();
                auto program = programCache.load(programPath, EPathType::Root);

                auto& declarationCache = VertexDeclarationCache::getSingleton();
                auto declaration = declarationCache.find(declarationName);

                if (declaration.isNull()) {
                    VertexDeclarationBuilder builder;
                    declaration = builder
                        .setName(declarationName)
                        .addBuffer("inPos")
                        .addElement("inPos", EVertexElementType::Float3)
                        .addBuffer("inWorldPos.inColor.inRadius", EVertexIterating::PerInstance)
                        .addElement("inWorldPos", EVertexElementType::Float3)
                        .addElement("inColor", EVertexElementType::Float3)
                        .addElement("inRadius", EVertexElementType::Float1)
                        .buildShared();
                }

                mCached = TPtrShared<Shader>::make(
                    "BatchedSphereShader",
                    program,
                    declaration
                );

                return mCached;
            }

        private:

            TPtrShared<Shader> mCached;
        };

    }
}

#endif //BERSERK_BATCHEDSPHERESHADER_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSTEXTURESHADER_H
#define BERSERK_GRAPHICSTEXTURESHADER_H

#include <ShaderProgramCache.h>
#include <ShaderFactory.h>
#include <RenderResources/VertexDeclarationCache.h>
#include <RenderResources/VertexDeclarationBuilder.h>

namespace Berserk {
    namespace Render {
        class GraphicsTextureShader : public ShaderFactory {
        public:

            GraphicsTextureShader() : ShaderFactory("GraphicsTextureShader") {

            }

            TPtrShared<Shader> create() override {
                if (mCached.isNotNull())
                    return mCached;

                const CString programPath = "Engine/Shaders/GraphicsTextureShader.json";
                const CString declarationName = "inPos.inTexCoords";

                auto& programCache = ShaderProgramCache::getSingleton();
                auto program = programCache.load(programPath, EPathType::Root);

                auto& declarationCache = VertexDeclarationCache::getSingleton();
                auto declaration = declarationCache.find(declarationName);

                if (declaration.isNull()) {
                    VertexDeclarationBuilder builder;
                    declaration = builder
                            .setName(declarationName)
                            .addBuffer("inPos.inTexCoords")
                            .addElement("inPos", EVertexElementType::Float3)
                            .addElement("inTexCoords", EVertexElementType::Float2)
                            .buildShared();
                }

                mCached = TPtrShared<Shader>::make(
                        "GraphicsTextureShader",
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

#endif //BERSERK_GRAPHICSTEXTURESHADER_H
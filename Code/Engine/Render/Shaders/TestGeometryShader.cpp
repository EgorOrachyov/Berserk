/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Shaders/TestGeometryShader.h>
#include <RenderResources/VertexDeclarationCache.h>
#include <ShaderCore/ShaderProgramCache.h>
#include <RenderResources/VertexDeclaration.h>
#include <RenderResources/VertexDeclarationBuilder.h>
#include <RenderResources/GraphicsPipelineBuilder.h>

namespace Berserk {
    namespace Render {

        TPtrShared<Shader> TestGeometryShader::create() {
            if (mCachedInstance.isNotNull())
                return mCachedInstance;

            const CString programPath = "Engine/Shaders/TestGeometry.json";
            const CString declarationName = "vsPosition.vsTexCoords|vsInstanceColor.vsInstancePosition.vsInstanceRadius";

            auto &programCache = ShaderProgramCache::getSingleton();
            auto &declarationCache = VertexDeclarationCache::getSingleton();

            auto program = programCache.load(programPath, EPathType::Root);
            BERSERK_COND_ERROR_RET_VALUE(nullptr, program.isNotNull(), "");

            auto declaration = declarationCache.find(declarationName);
            if (declaration.isNull()) {
                VertexDeclarationBuilder declarationBuilder;

                declaration = declarationBuilder
                    .setName(declarationName)
                    .addBuffer("vsPosition.vsTexCoords")
                    .addElement("vsPosition", EVertexElementType::Float3)
                    .addElement("vsTexCoords", EVertexElementType::Float2)
                    .addBuffer("vsInstanceColor.vsInstancePosition.vsInstanceRadius", EVertexIterating::PerInstance)
                    .addElement("vsInstanceColor", EVertexElementType::Float3)
                    .addElement("vsInstancePosition", EVertexElementType::Float3)
                    .addElement("vsInstanceRadius", EVertexElementType::Float1)
                    .buildShared();
            }

            BERSERK_COND_ERROR_RET_VALUE(nullptr, declaration.isNotNull() && declaration->isInitialized(), "Failed to create declaration");
            VertexDeclarationCache::getSingleton().cache(declaration);

            mCachedInstance = TPtrShared<Shader>::make(
                "TestGeometry",
                program,
                declaration
            );

            return mCachedInstance;
        }
    }
}
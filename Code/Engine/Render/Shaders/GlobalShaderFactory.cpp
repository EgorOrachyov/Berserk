/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Shaders/GlobalShaderFactory.h>
#include <ShaderProgramCache.h>
#include <ShaderProgramCompiler.h>
#include <VertexDeclarationCache.h>
#include <VertexDeclarationLoader.h>

namespace Berserk {
    namespace Render {

        GlobalShaderFactory::GlobalShaderFactory() : ShaderFactory("Global") {

        }

        TPtrShared<Shader> GlobalShaderFactory::create() {
            return nullptr;
        }

        TPtrShared<Shader> GlobalShaderFactory::create(const CString &shaderName) {
            if (mCached.contains(shaderName)) {
                return mCached[shaderName];
            }

            CString relativeFilePath = CString{"Engine/Shaders/"} + shaderName + ".json";

            ShaderFile shaderFile(relativeFilePath, mPathType);
            ShaderProgramCompiler programCompiler(shaderName, shaderFile);

            BERSERK_COND_ERROR_RET_VALUE(nullptr, shaderFile.hasVertexDeclaration(), "Vertex declaration is not provided for %s", shaderFile.getFilePath().data());

            if (!programCompiler.canCompile()) {
                BERSERK_ERROR("Cannot compile shader program %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            programCompiler.compile();

            if (!programCompiler.isCompiled()) {
                BERSERK_ERROR("Failed to compile shader program %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            if (!programCompiler.canCreateProgram()) {
                BERSERK_ERROR("Cannot create shader program instance %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return nullptr;
            }

            auto program = programCompiler.createProgram();

            auto& declarationValue = shaderFile.getVertexDeclaration();
            auto& name = declarationValue.getDict()["Name"].getString();
            auto& declarationCache = VertexDeclarationCache::getSingleton();

            TPtrShared<VertexDeclaration> declaration;

            if (declarationCache.contains(name)) {
                declaration = declarationCache.find(name);
            }
            else {
                VertexDeclarationLoader loader(declarationValue);
                loader.load();
                declaration = loader.getDeclaration();
                declarationCache.cache(declaration);
                declaration->showDebugInfo();
            }

            TPtrShared<Shader> shader = TPtrShared<Shader>::make(
                shaderName,
                program,
                declaration
            );

            mCached.add(shaderName, shader);
            return shader;
        }
    }
}
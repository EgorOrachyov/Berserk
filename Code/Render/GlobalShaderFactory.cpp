/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GlobalShaderFactory.h>
#include <GlobalShaderFactoryOptions.h>
#include <ShaderDefinitions.h>
#include <ShaderProgramCache.h>
#include <ShaderProgramCompiler.h>
#include <VertexDeclarationCache.h>
#include <VertexDeclarationLoader.h>

namespace Berserk {
    namespace Render {

        GlobalShaderFactory::GlobalShaderFactory() : ShaderFactory("Global") {
            auto& defs = ShaderDefinitions::getSingleton();

            // Add global shaders definitions here
        }

        TPtrShared<Shader> GlobalShaderFactory::create() {
            return nullptr;
        }

        TPtrShared<Shader> GlobalShaderFactory::create(const ShaderFactoryOptions &options) {
            auto globalFactoryOptions = dynamic_cast<const GlobalShaderFactoryOptions*>(&options);
            BERSERK_COND_ERROR_RET_VALUE(nullptr, globalFactoryOptions != nullptr, "Provided invalid options");

            auto shaderName = globalFactoryOptions->getShaderName();

            if (mCached.contains(shaderName)) {
                return mCached[shaderName];
            }

            CString relativeFilePath = CString{"Engine/Shaders/"} + shaderName + ".json";

            // Global definitions always passed to this factory shaders
            ShaderCompilerInsertions compilerInsertions;
            ProcessorInsertionsGlsl insertionsGlsl;
            {
                auto& defs = ShaderDefinitions::getSingleton();
                mCachedDefinitions.clear();

                for (auto& entry: defs.getDefinitions()) {
                    auto& name = entry.first();
                    auto& value = entry.second().value;
                    auto& mask = entry.second().mask;

                    if (mask.getFlag(EDefinitionFlag::Global)) {
                        mCachedDefinitions.add(name + " " + value.toString().removeSymbols("\""));
                    }
                }

                insertionsGlsl.definitions = &mCachedDefinitions;

                compilerInsertions.vertex = insertionsGlsl;
                compilerInsertions.fragment = insertionsGlsl;
            }

            ShaderFile shaderFile(relativeFilePath, mPathType);
            ShaderProgramCompiler programCompiler(shaderName, shaderFile, compilerInsertions);

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
            }

#if 0
            declaration->showDebugInfo();
            program->getMetaData()->showDebugInfo();
#endif

            TPtrShared<Shader> shader = TPtrShared<Shader>::make(
                std::move(name),
                std::move(program),
                std::move(declaration)
            );

            mCached.add(shaderName, shader);
            return shader;
        }
    }
}
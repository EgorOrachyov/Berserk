/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <MaterialShaderFactory.h>
#include <MaterialShaderFactoryOptions.h>
#include <ShaderDefinitions.h>
#include <ShaderProgramCache.h>
#include <ShaderProgramCompiler.h>
#include <VertexDeclarationCache.h>
#include <GpuMeshDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        MaterialShaderFactory* MaterialShaderFactory::gFactory = nullptr;

        MaterialShaderFactory::MaterialShaderFactory() : ShaderFactory("Material") {
            BERSERK_COND_ERROR_RET(gFactory == nullptr, "Allowed only single MaterialShaderFactory instance");
            gFactory = this;

            // Create dummy shader to have access to uniform layout of common blocks

            MaterialFeatures features = { };
            MeshFormat format = { EMeshAttribute::Position, EMeshAttribute::Normal, EMeshAttribute::Tangent, EMeshAttribute::Color, EMeshAttribute::TexCoord };
            EMaterialShading shading = EMaterialShading::BlinnPhong;
            LightsConsts lightsConsts{};

            ShaderCompilerInsertions insertions;
            ProcessorInsertionsGlsl vertex;
            ProcessorInsertionsGlsl fragment;
            {
                auto& defs = ShaderDefinitions::getSingleton();
                mSharedDefinitionsList.clear();
                mVertexShaderCodeInsertions.clear();

                for (auto& entry: defs.getDefinitions()) {
                    auto& name = entry.first();
                    auto& value = entry.second().value;
                    auto& mask = entry.second().mask;

                    if (mask.getFlag(EDefinitionFlag::Global) || mask.getFlag(EDefinitionFlag::Material)) {
                        mSharedDefinitionsList.add(name + " " + value.toString().removeSymbols("\""));
                    }
                }

                prepareShaderInsData(shading, features, format, lightsConsts);

                vertex.sharedCode = &mVertexShaderCodeInsertions;
                vertex.definitions = &mSharedDefinitionsList;
                fragment.sharedCode = nullptr;
                fragment.definitions = &mSharedDefinitionsList;

                insertions.vertex = vertex;
                insertions.fragment = fragment;
            }

            EPathType pathType = EPathType::Root;
            CString shaderName = "Material";
            CString relativeFilePath = "Engine/Shaders/material.json";

            ShaderFile shaderFile(relativeFilePath, pathType);
            ShaderProgramCompiler programCompiler(shaderName, shaderFile, insertions);

            if (!programCompiler.canCompile()) {
                BERSERK_ERROR("Cannot compile shader program %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return;
            }

            programCompiler.compile();

            if (!programCompiler.isCompiled()) {
                BERSERK_ERROR("Failed to compile shader program %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return;
            }

            if (!programCompiler.canCreateProgram()) {
                BERSERK_ERROR("Cannot create shader program instance %s: %s", shaderFile.getFilePath().data(), programCompiler.getInfoMessage().data());
                return;
            }

            mDummyProgram = programCompiler.createProgram();

#if 1
            mDummyProgram->getMetaData()->showDebugInfo();
#endif
        }

        MaterialShaderFactory::~MaterialShaderFactory() {
            gFactory = nullptr;
        }

        TPtrShared<Shader> MaterialShaderFactory::create() {
            return nullptr;
        }

        TPtrShared<Shader> MaterialShaderFactory::create(const ShaderFactoryOptions &options) {
            auto materialFactoryOptions = dynamic_cast<const MaterialShaderFactoryOptions*>(&options);
            BERSERK_COND_ERROR_RET_VALUE(nullptr, materialFactoryOptions != nullptr, "Invalid options provided");

            const TPtrShared<Material> &material = materialFactoryOptions->getMaterial();
            const MeshFormat& format = materialFactoryOptions->getMeshFormat();

            BERSERK_COND_ERROR_RET_VALUE(nullptr, material.isNotNull(), "Provided null material");
            BERSERK_COND_ERROR_RET_VALUE(nullptr, format.getFlag(EMeshAttribute::Position), "Position attribute is required for vertex");

            MaterialFeatures features;
            MaterialFlags flags = material->getFlags();
            MaterialKey materialKey;
            {
                extractMaterialFeatures(features, *material, format);
                materialKey = MaterialKey(format, features, flags, material->getType(), material->getShading());

                auto ptr = mCachedShaders.getPtr(materialKey);
                if (ptr.isNotNull()) {
                    return ptr->castTo<Shader>();
                }
            }

            ShaderCompilerInsertions insertions;
            ProcessorInsertionsGlsl vertex;
            ProcessorInsertionsGlsl fragment;
            {
                auto& defs = ShaderDefinitions::getSingleton();
                mSharedDefinitionsList.clear();
                mVertexShaderCodeInsertions.clear();

                for (auto& entry: defs.getDefinitions()) {
                    auto& name = entry.first();
                    auto& value = entry.second().value;
                    auto& mask = entry.second().mask;

                    if (mask.getFlag(EDefinitionFlag::Global) || mask.getFlag(EDefinitionFlag::Material)) {
                        mSharedDefinitionsList.add(name + " " + value.toString().removeSymbols("\""));
                    }
                }

                prepareShaderInsData(material->getShading(), features, format, materialFactoryOptions->getLightsConsts());

                vertex.sharedCode = &mVertexShaderCodeInsertions;
                vertex.definitions = &mSharedDefinitionsList;
                fragment.sharedCode = nullptr;
                fragment.definitions = &mSharedDefinitionsList;

                insertions.vertex = vertex;
                insertions.fragment = fragment;
            }

            EPathType pathType = EPathType::Root;
            CString shaderName = "Material";
            CString relativeFilePath = "Engine/Shaders/material.json";

            ShaderFile shaderFile(relativeFilePath, pathType);
            ShaderProgramCompiler programCompiler(shaderName, shaderFile, insertions);

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

            auto& declarationCache = VertexDeclarationCache::getSingleton();
            TPtrShared<VertexDeclaration> declaration;

            auto declarationName = MeshFormatUtil::getFormatAsDeclarationString(format);

            if (declarationCache.contains(declarationName)) {
                declaration = declarationCache.find(declarationName);
            }
            else {
                GpuMeshDeclarationBuilder builder;
                builder.setMeshFormat(format);
                declaration = builder.buildShared();
                declarationCache.cache(declaration);
            }
#if 0
            declaration->showDebugInfo();
            program->getMetaData()->showDebugInfo();
#endif

            TPtrShared<MaterialShader> materialShader = TPtrShared<MaterialShader>::make(
                    std::move(shaderName),
                    std::move(program),
                    std::move(declaration)
            );

            mCachedShaders.add(materialKey,materialShader);
            return materialShader.castTo<Shader>();
        }

        void MaterialShaderFactory::extractMaterialFeatures(MaterialFeatures &features, const Material &material, const MeshFormat &format) {
            features = material.getFeatures();
            MaterialFeatures mask(0xffffffffff); // Suppose has all features

            if (!format.getFlag(EMeshAttribute::Normal))
                mask ^= MaterialFeatures({ EMaterialFeature::Normal, EMaterialFeature::Displacement } );

            if (!format.getFlag(EMeshAttribute::TexCoord))
                mask &= 0x0;

            features &= mask;
        }

        void MaterialShaderFactory::prepareShaderInsData(EMaterialShading materialShading, const MaterialFeatures& features, const MeshFormat &format, const LightsConsts& lightsConsts) {

            static const char* featuresNames[] = {
                "FEATURE_ALBEDO",
                "FEATURE_SPECULAR",
                "FEATURE_METALLIC",
                "FEATURE_ROUGHNESS",
                "FEATURE_NORMAL",
                "FEATURE_DISPLACEMENT",
                "FEATURE_AMBIENT_OCCLUSION",
                "FEATURE_EMISSION"
            };

            for (uint32 i = 0; i < (uint32)EMaterialFeature::Max; i++) {
                if (features.getFlag((EMaterialFeature)i)) {
                    mSharedDefinitionsList.add(featuresNames[i]);
                }
            }

            static const char* attributesNames[] {
                "ATTRIBUTE_POSITION",
                "ATTRIBUTE_NORMAL",
                "ATTRIBUTE_TANGENT",
                "ATTRIBUTE_COLOR",
                "ATTRIBUTE_TEXTURE_COORDS"
            };

            for (uint32 i = 0; i < (uint32)EMeshAttribute::Max; i++) {
                if (format.getFlag((EMeshAttribute)i)) {
                    mSharedDefinitionsList.add(attributesNames[i]);
                }
            }

            static const char* shadingNames[] = {
                "SHADING_BLINN_PHONG",
                "SHADING_PBR",
                "CUSTOM"
            };

            for (uint32 i = 0; i < (uint32)EMaterialShading::Max; i++) {
                if (materialShading == (EMaterialShading)i) {
                    mSharedDefinitionsList.add(shadingNames[i]);
                    break;
                }
            }

            mSharedDefinitionsList.add(CString{"LIGHT_DIRECTIONAL "} + CString::fromUint32(lightsConsts.typeDirectionalLight));
            mSharedDefinitionsList.add(CString{"LIGHT_SPOT "} + CString::fromUint32(lightsConsts.typeSpotLight));
            mSharedDefinitionsList.add(CString{"LIGHT_POINT "} + CString::fromUint32(lightsConsts.typePointLight));
            mSharedDefinitionsList.add(CString{"MAX_LIGHTS_PER_PIXEL "} + CString::fromUint32(lightsConsts.lightsPerPixel));
            mSharedDefinitionsList.add(CString{"MAX_LIGHTS_PER_SCENE "} + CString::fromUint32(lightsConsts.lightsPerScene));

            {
                uint32 currentLocation = 0;

                for (uint32 i = 0; i < (uint32)EMeshAttribute::Max; i++) {
                    auto attribute = (EMeshAttribute)i;

                    if (format.getFlag(attribute)) {
                        mVertexCodeBuilder.empty();

                        CString location = CString::fromUint32(currentLocation);

                        // GLSL style input attributes.
                        // HLSL case must be handled separately
                        mVertexCodeBuilder.append("layout (location = ");
                        mVertexCodeBuilder.append(location);
                        mVertexCodeBuilder.append(" ) in ");
                        mVertexCodeBuilder.append(RHIDefinitionsUtil::getVertexElementStringAsGlslType(MeshFormatUtil::getAttributeType(attribute)));
                        mVertexCodeBuilder.append(" ");
                        mVertexCodeBuilder.append(MeshFormatUtil::getAttributeDeclarationNameFromEnum(attribute));
                        mVertexCodeBuilder.append(";");

                        mVertexShaderCodeInsertions.add(mVertexCodeBuilder.toString());
                        currentLocation += 1;
                    }
                }
            }

#if 0
            printf("Definitions:\n");
            for (const auto& d: defs) {
                printf("%s\n",d.data());
            }
            printf("Locations:\n");
            for (const auto& c: code) {
                printf("%s\n", c.data());
            }
#endif
        }

        TRef<const ShaderUniformBlock> MaterialShaderFactory::getCameraDataLayout() {
            return gFactory ? gFactory->mDummyProgram->getMetaData()->getUniformBlock("CameraData") : nullptr;
        }

        TRef<const ShaderUniformBlock> MaterialShaderFactory::getMaterialDataLayout() {
            return gFactory ? gFactory->mDummyProgram->getMetaData()->getUniformBlock("MaterialData") : nullptr;
        }

        TRef<const ShaderUniformBlock> MaterialShaderFactory::getTransformDataLayout() {
            return gFactory ? gFactory->mDummyProgram->getMetaData()->getUniformBlock("TransformData") : nullptr;
        }

    }
}
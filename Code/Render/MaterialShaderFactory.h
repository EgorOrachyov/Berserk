/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALSHADERFACTORY_H
#define BERSERK_MATERIALSHADERFACTORY_H

#include <ShaderFactory.h>
#include <String/CStringBuilder.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <MaterialKey.h>
#include <MaterialShader.h>
#include <MaterialShaderFactoryOptions.h>

namespace Berserk {
    namespace Render {

        /** Factory for compiling and  creating material shaders with embedded vertex attributes and features */
        class MaterialShaderFactory : public ShaderFactory {
        public:
            /** Default */
            MaterialShaderFactory();
            ~MaterialShaderFactory();

            /** Not supported */
            TPtrShared<Shader> create() override;

            /** Creates material shader from options passed */
            TPtrShared<Shader> create(const ShaderFactoryOptions &options) override;

            /** Standard layout of the material shader data */
            static TRef<const ShaderUniformBlock> getCameraDataLayout();

            /** Standard layout of the material shader data */
            static TRef<const ShaderUniformBlock> getTransformDataLayout();

            /** Standard layout of the material shader data */
            static TRef<const ShaderUniformBlock> getMaterialDataLayout();

        private:

            void extractMaterialFeatures(MaterialFeatures& features, const Material& material, const MeshFormat &format);
            void prepareShaderInsData(EMaterialShading materialShading, const MaterialFeatures& features, const MeshFormat &format, const LightsConsts& lightsConsts);

            /** Cached shaders per material/geometry type */
            TMap<MaterialKey,TPtrShared<MaterialShader>> mCachedShaders;
            TArray<CString> mSharedDefinitionsList;
            TArray<CString> mVertexShaderCodeInsertions;
            CStringBuilder mVertexCodeBuilder;
            TPtrShared<ShaderProgram> mDummyProgram;

            static MaterialShaderFactory* gFactory;
        };

    }
}



#endif //BERSERK_MATERIALSHADERFACTORY_H

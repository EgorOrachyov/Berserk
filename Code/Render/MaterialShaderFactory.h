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
#include <MaterialShader.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <String/CStringBuilder.h>

namespace Berserk {
    namespace Render {

        /** Factory for compiling and  creating material shaders with embedded vertex attributes and features */
        class MaterialShaderFactory : public ShaderFactory {
        public:
            /** Default */
            MaterialShaderFactory();

            /** Not supported */
            TPtrShared<Shader> create() override;

            /** Creates material shader from options passed */
            TPtrShared<Shader> create(const ShaderFactoryOptions &options) override;

        private:

            /** Extracts definitions from material and vertex format, to pass then to shader compiler */
            void extractDefinitions(const Material &material, const MeshFormat &format, TArray <CString> &defs, TArray <CString> &code);

            /** Cached shader per material name (for each store with different ) */
            TMap<CString,TArray<TPtrShared<MaterialShader>>> mCachedShaders;
            TArray<CString> mSharedDefinitionsList;
            TArray<CString> mVertexShaderCodeInsertions;
            CStringBuilder mVertexCodeBuilder;
        };

    }
}



#endif //BERSERK_MATERIALSHADERFACTORY_H

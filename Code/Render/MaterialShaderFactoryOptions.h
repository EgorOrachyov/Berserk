/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALSHADERFACTORYOPTIONS_H
#define BERSERK_MATERIALSHADERFACTORYOPTIONS_H

#include <ShaderFactory.h>
#include <Material.h>
#include <GpuMeshAttribute.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        struct LightsConsts {
            uint32 lightsPerScene = 100;
            uint32 lightsPerPixel = 100;
            uint32 typeDirectionalLight = 0;
            uint32 typeSpotLight = 1;
            uint32 typePointLight = 2;
        };

        /** Options to create material shader */
        class MaterialShaderFactoryOptions : public ShaderFactoryOptions {
        public:
            void setMaterial(TPtrShared<Material> m) { material = std::move(m); }
            void setMeshFormat(const MeshFormat& f) { format = f; }
            void setLightConsts(const LightsConsts& c) { lightsConsts = c; }

            const TPtrShared<Material>& getMaterial() const { return material; }
            const MeshFormat &getMeshFormat() const { return format; }
            const LightsConsts &getLightsConsts() const { return lightsConsts; }

        private:
            TPtrShared<Material> material;
            MeshFormat format;
            LightsConsts lightsConsts;

        };

    }
}

#endif //BERSERK_MATERIALSHADERFACTORYOPTIONS_H
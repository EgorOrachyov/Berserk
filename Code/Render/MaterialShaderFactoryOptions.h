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
#include <RenderResources/GpuMeshAttribute.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /** Options to create material shader */
        class MaterialShaderFactoryOptions : public ShaderFactoryOptions {
        public:
            void setMaterial(TPtrShared<Material> m) { material = std::move(m); }
            void setMeshFormat(const MeshFormat& f) { format = f; }

            const TPtrShared<Material>& getMaterial() const { return material; }
            const MeshFormat &getMeshFormat() const { return format; }

        private:
            TPtrShared<Material> material;
            MeshFormat format;
        };

    }
}

#endif //BERSERK_MATERIALSHADERFACTORYOPTIONS_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GPUMESHDECLARATIONBUILDER_H
#define BERSERK_GPUMESHDECLARATIONBUILDER_H

#include <RenderResources/GpuMeshAttribute.h>
#include <RenderResources/VertexDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        /** Declaration builder for mesh format based declarations */
        class GpuMeshDeclarationBuilder {
        public:

            /** Specify */
            GpuMeshDeclarationBuilder& setMeshFormat(const MeshFormat& format);

            /** Build declaration resource */
            TPtrShared<VertexDeclaration> buildShared();

        private:
            bool mMeshFormatSpecified = false;
            MeshFormat mMeshFormat;
            VertexDeclarationBuilder mBuilder;
        };

    }
}

#endif //BERSERK_GPUMESHDECLARATIONBUILDER_H
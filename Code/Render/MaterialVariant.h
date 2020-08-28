/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALVARIANT_H
#define BERSERK_MATERIALVARIANT_H

#include <Typedefs.h>
#include <GpuMeshFormat.h>

namespace Berserk {
    namespace Render {

        /** Flags set for material shaders fetching */
        class MaterialVariant {
        public:
            static const uint16 POSITION = (uint16) EMeshAttribute::Position;
            static const uint16 NORMAL = (uint16) EMeshAttribute::Normal;
            static const uint16 TANGENT = (uint16) EMeshAttribute::Tangent;
            static const uint16 COLOR = (uint16) EMeshAttribute::Color;
            static const uint16 TEX_COORDS = (uint16) EMeshAttribute::TexCoord;

            static const uint16 TANGENT_SPACE = NORMAL | TANGENT;
            static const uint16 VERTEX_COLOR = COLOR;

            MaterialVariant(const MeshFormat& format) { mKey = (uint16) format.getMask(); }

            bool hasNormal() const { return NORMAL & mKey; }
            bool hasVertexColor() const { return VERTEX_COLOR & mKey; }
            bool hasTangentSpace() const { return (TANGENT_SPACE & mKey) == TANGENT_SPACE; }

            uint16 getKey() const { return mKey; }

        private:

            uint16 mKey = 0;
        };

    }
}

#endif //BERSERK_MATERIALVARIANT_H
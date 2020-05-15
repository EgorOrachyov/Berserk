/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXBUFFER_H
#define BERSERK_VERTEXBUFFER_H

#include <Rendering/Resources/IRenderResource.h>

namespace Berserk {
    namespace Rendering {

        /** Arbitrary vertex buffer for RHI vertex data access */
        class VertexBuffer : public IRenderResource {
        public:
            ~VertexBuffer() override = default;
        };

    }
}

#endif //BERSERK_VERTEXBUFFER_H
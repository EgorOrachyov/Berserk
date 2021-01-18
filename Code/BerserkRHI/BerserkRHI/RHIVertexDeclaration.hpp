/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIVERTEXDECLARATION_HPP
#define BERSERK_RHIVERTEXDECLARATION_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Containers/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        /**	Describes a single vertex element in a vertex declaration. */
        class VertexElement {
            uint32 offset;
            uint32 stride;
            uint8 buffer;
            uint8 index;
            VertexElementType type;
            VertexIterating iterating;
        };

        /**	Contains information about a vertex declaration. */
        class VertexDeclaration: public Resource {
        public:
            using Desc = ArrayFixed<VertexElement, Limits::MAX_VERTEX_ATTRIBUTES>;

            ~VertexDeclaration() override = default;

            /** @return Elements declarations */
            const Desc &GetElements() const { return mAttributes; }

        protected:
            /** Attributes, used in the declaration */
            Desc mAttributes;
        };

    }
}

#endif //BERSERK_RHIVERTEXDECLARATION_HPP
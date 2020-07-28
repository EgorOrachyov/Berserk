/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHIVERTEXDECLARATION_H
#define BERSERK_RHIVERTEXDECLARATION_H

#include <RHI/RHIResource.h>

namespace Berserk {

    class RHIVertexDeclaration : public RHIResource {
    public:
        ~RHIVertexDeclaration() override = default;

        /** @return Total input locations in vertex shader */
        uint32 getLocationsUses() const { return mLocationsUses; }

        /** @return Total separate buffers usage */
        uint32 getBuffersUses() const { return mBuffersUses; }

        /** @return Instancing usage for this layout */
        bool getUsesInstancing() const { return mUsesInstancing; }

        /** @return Elements declarations */
        const TArrayStatic<RHIVertexElement,RHIConst::MAX_VERTEX_ATTRIBUTES> &getElements() const { return mElementsDeclarations; }

    protected:
        /** Total input locations in vertex shader */
        uint32 mLocationsUses = 0;

        /** Total separate buffers usage */
        uint32 mBuffersUses = 0;

        /** Instancing usage for this layout */
        bool mUsesInstancing = false;

        /** Elements info */
        TArrayStatic<RHIVertexElement,RHIConst::MAX_VERTEX_ATTRIBUTES> mElementsDeclarations;

    };

}

#endif //BERSERK_RHIVERTEXDECLARATION_H

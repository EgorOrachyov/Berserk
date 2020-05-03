/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESH_H
#define BERSERK_MESH_H

#include <TArray.h>
#include <Resource.h>
#include <RHI/RHIDefinitions.h>
#include <Rendering/VertexPolicy.h>

namespace Berserk {

    /** Raw geometry container for indexed mesh data with of the engine vertices layouts */
    class Mesh : public Resource {
    public:

        ~Mesh() override = default;

        /** @return How vertices are arranged */
        EPrimitivesType getPrimitivesType() const { return mPrimitivesType; }

        /** @return Layout of the data in the mesh */
        const TPtrShared<VertexPolicy> &getVertexPolicy() { return mVertexPolicy; }

        /** @return Raw vertex data */
        const TArray<uint8> &getVerticesData() const { return mVerticesData; }

        /** @return Uint32 based indices */
        const TArray<uint32> &getIndicesData() const { return mIndicesData; }

        /** @return True if mesh uses indices */
        bool getIsIndexed() const { return mIsIndexed; }

        /** @return Number of the vertices in the mesh */
        uint32 getVerticesCount() const { return mVerticesCount; }

    private:

        /** How vertices are arranged */
        EPrimitivesType mPrimitivesType;

        /** Layout of the data in the mesh */
        TPtrShared<VertexPolicy> mVertexPolicy;

        /** Raw vertex data */
        TArray<uint8> mVerticesData;

        /** Uint32 based indices */
        TArray<uint32> mIndicesData;

        /** Number of the vertices in the mesh */
        uint32 mVerticesCount;

        /** This mesh uses indices */
        bool mIsIndexed = false;
    };

}

#endif //BERSERK_MESH_H
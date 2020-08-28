/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STATICMESH_H
#define BERSERK_STATICMESH_H

#include <Math/Aabbf.h>
#include <Math/Transformf.h>
#include <Scene/RenderObject.h>
#include <Containers/TArrayView.h>
#include <GpuMeshFormat.h>
#include <RHI/RHIIndexBuffer.h>
#include <RHI/RHIVertexBuffer.h>
#include <Scene/StaticMeshLod.h>
#include <Scene/StaticMeshNode.h>
#include <MaterialInstance.h>

namespace Berserk {
    namespace Render {

        /** Represent single node with material and data view, packed inside LOD data */
        struct StaticMeshNodeData {
            CString name;
            Aabbf aabb;
            uint32 offset = 0;
            uint32 count = 0;
            TPtrShared<MaterialInstance> material;
        };

        /** Represent single mesh lod level, composed from nodes */
        struct StaticMeshLodData {
            EPrimitivesType primitivesType;
            EIndexType indexType = EIndexType::Unknown;
            uint32 verticesCount = 0;
            uint32 indicesCount = 0;
            MeshFormat format;
            TArrayView<uint8> vertexData;
            TArrayView<uint8> indexData;
            TArrayView<StaticMeshNodeData> nodes;
        };

        /**
         * @brief Static mesh
         *
         * Represents static render mesh, with optional levels of detail.
         * Mesh could be composed from the nodes, each node could be rendered
         * with custom material.
         */
        class StaticMesh final : public RenderObject {
        public:
            /** Max level of details for the static mesh */
            static const uint32 MAX_LOD = 4;

            /**
             * Create static mesh from data for each lod level
             * @param bounds Mesh overall bounds in local space, used for culling
             * @param data Per LOD mesh nodes and packed vertex/index data
             */
            StaticMesh(const Aabbf &bounds, const TArrayStatic<StaticMeshLodData, MAX_LOD> &data);

            ~StaticMesh() override = default;

            /** @copydoc RenderObject::onOcclusionQueryResults() */
            void onOcclusionQueryResults(const SceneView &sceneView, const TArrayView<bool> &results) override;

            /** @return LODs count */
            const uint32 getLodsCount() const { return mLods.size(); }

            /** @return LOD info at index */
            const StaticMeshLod& getLod(uint32 index) const { return mLods[index]; }

            /** @return Array view for nodes of chosen lod level */
            TArrayView<StaticMeshNode> getNodesForLod(uint32 index);

        private:

            /** Nodes among all LOD levels */
            TArray<StaticMeshNode> mNodes;

            /** Info per each lod */
            TArrayStatic<StaticMeshLod,MAX_LOD> mLods;
        };

    }
}

#endif //BERSERK_STATICMESH_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Scene/StaticMesh.h>
#include <RHI/RHIDevice.h>
#include "SceneView.h"

namespace Berserk {
    namespace Render {

        StaticMesh::StaticMesh(const Aabbf &bounds, const TArrayStatic<StaticMeshLodData, MAX_LOD> &data) {
            auto& device = RHIDevice::getSingleton();

            mBounds = bounds;
            uint32 lodLevel = 0;

            // Collect nodes, fill RHI vertex data and allocate index buffers in needed
            for (auto& dataLod: data) {
                StaticMeshLod lod; {
                    lod.nodesOffset = mNodes.size();
                    lod.nodesCount = dataLod.nodes.size();
                    lod.format = dataLod.format;
                    lod.primitivesType = dataLod.primitivesType;
                    lod.verticesCount = dataLod.verticesCount;

                    uint32 stride = MeshFormatUtil::getStrideSizeForFormat(dataLod.format);
                    uint32 dataSize = stride * dataLod.verticesCount;

                    // Explicitly check
                    BERSERK_COND_ERROR_RET(dataSize == dataLod.vertexData.size(), "Data size from format does not match buffer size");

                    lod.vertexData = device.createVertexBuffer(dataSize, EBufferUsage::Static, dataLod.vertexData.data());
                }

                mLods.add(lod);

                bool isIndexed = dataLod.indicesCount > 0;
                EIndexType indexType = dataLod.indexType;
                uint32 indexSize = RHIDefinitionsUtil::getIndexSize(indexType);

                for (auto& dataNode: dataLod.nodes) {
                    StaticMeshNode node; {
                        node.name = dataNode.name;
                        node.aabbf = dataNode.aabb;
                        node.material = dataNode.material;

                        node.isIndexed = isIndexed;
                        node.lod = lodLevel;

                        if (isIndexed) {
                            uint32 indicesCount = dataNode.count;
                            uint32 indicesOffset = dataNode.offset;
                            uint32 indicesCopyOffset = indexSize * indicesOffset;
                            uint32 indicesCopySize = indexSize * indicesCount;

                            // Explicitly check
                            BERSERK_COND_ERROR_RET(indicesCopyOffset + indicesCopySize <= dataLod.indexData.size(), "Out of buffer bounds index region");

                            node.offset = 0;
                            node.count = dataNode.count;
                            node.indexType = indexType;
                            node.indexData = device.createIndexBuffer(indicesCopySize, EBufferUsage::Static, dataLod.indexData.data() + indicesCopyOffset);
                        }
                        else {
                            node.offset = dataNode.offset;
                            node.count = dataNode.count;
                        }
                    }

                    mNodes.emplace(std::move(node));
                }

                lodLevel += 1;
            }

        }

        void StaticMesh::onOcclusionQueryResults(const SceneView &sceneView, const TArrayView<bool> &results) {

        }

        TArrayView<StaticMeshNode> StaticMesh::getNodesForLod(Berserk::uint32 index) {
            auto& lod = mLods[index];
            return TArrayView<StaticMeshNode>(mNodes, lod.nodesOffset, lod.nodesCount);
        }
    }
}
//
// Created by Egor Orachyov on 2019-08-15.
//

#include "RenderUtils.h"

namespace Berserk
{

    void RenderUtils::createGeometryBuffer(RHIDriverRef &driver, RenderableSceneInfoRef &data)
    {
        HMesh &mesh = data->renderable->getMesh();

        const TArray<uint8> &vertices = mesh->getVerticesBuffer();
        const TArray<uint8> &indices = mesh->getIndicesBuffer();

        auto vertexBuffer = driver->createVertexBuffer(
                indices.getSize(),
                indices.getRawBuffer(),
                mesh->getVertexBufferUsage());

        auto indexBuffer = driver->createIndexBuffer(
                indices.getSize(),
                indices.getRawBuffer(),
                mesh->getIndexBufferUsage(),
                mesh->getIndicesType());

        data->geometry = driver->createGeometryBuffer(
                vertexBuffer,
                indexBuffer,
                mesh->getVerticesType(),
                mesh->getPrimitiveType());
    }

} // namespace Berserk
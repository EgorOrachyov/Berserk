/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <core/Engine.hpp>
#include <render/mesh/Mesh.hpp>
#include <utility>

BRK_NS_BEGIN

Mesh::Mesh(MeshFormat format, uint32 verticesCount, const Ref<Data> &vertexData, const Ref<Data> &attributeData, const Ref<Data> &skinningData) {
    InitFromData(format, verticesCount, vertexData, attributeData, skinningData);
}

Mesh::Mesh(MeshFormat format, uint32 verticesCount, const MeshArrays &meshArrays) {
    if (!verticesCount) {
        BRK_ERROR("Cannot create mesh without vertices");
        return;
    }
    if (!format.Get(MeshAttribute::Position)) {
        BRK_ERROR("Cannot create mesh without vertex position data");
        return;
    }

    auto &formats = Engine::Instance().GetRenderEngine().GetMeshFormats();

    uint32 strideVertex, strideAttribute, strideSkinning;
    formats.GetStride(format, strideVertex, strideAttribute, strideSkinning);

    auto copyDataPacked = [&](MeshAttribute attribute, uint32 stride, unsigned char *dst, const unsigned char *src) {
        if (format.Get(attribute)) {
            if (!src) {
                BRK_WARNING("Mesh attribute is set but not provided");
                return;
            }

            auto dstOffset = formats.GetAttributeOffset(attribute, format);
            auto srcOffset = 0u;
            auto size = MeshGetAttributeInfo(attribute).size;
            for (std::size_t i = 0; i < verticesCount; i++) {
                Memory::Copy(dst + dstOffset, src + srcOffset, size);
                dstOffset += stride;
                srcOffset += size;
            }
        }
    };

    Ref<Data> vertexData;
    Ref<Data> attributeData;
    Ref<Data> skinningData;

    if (strideVertex) {
        vertexData = Data::Make(verticesCount * strideVertex);
        copyDataPacked(MeshAttribute::Position, strideVertex, reinterpret_cast<unsigned char *>(vertexData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.positions));
        copyDataPacked(MeshAttribute::Normal, strideVertex, reinterpret_cast<unsigned char *>(vertexData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.normals));
        copyDataPacked(MeshAttribute::Tangent, strideVertex, reinterpret_cast<unsigned char *>(vertexData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.tangents));
    }
    if (strideAttribute) {
        attributeData = Data::Make(verticesCount * strideAttribute);
        copyDataPacked(MeshAttribute::Color, strideAttribute, reinterpret_cast<unsigned char *>(attributeData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.colors));
        copyDataPacked(MeshAttribute::UV, strideAttribute, reinterpret_cast<unsigned char *>(attributeData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.uvs));
        copyDataPacked(MeshAttribute::UV2, strideAttribute, reinterpret_cast<unsigned char *>(attributeData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.uvs2));
    }
    if (strideSkinning) {
        skinningData = Data::Make(verticesCount * strideSkinning);
        copyDataPacked(MeshAttribute::Weights, strideSkinning, reinterpret_cast<unsigned char *>(skinningData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.weights));
        copyDataPacked(MeshAttribute::Bones, strideSkinning, reinterpret_cast<unsigned char *>(skinningData->GetDataWrite()), reinterpret_cast<const unsigned char *>(meshArrays.bones));
    }

    InitFromData(format, verticesCount, vertexData, attributeData, skinningData);
}


void Mesh::SetName(StringName name) {
    mName = std::move(name);
}

void Mesh::SetAabb(const Aabbf &aabb) {
    mAabb = aabb;
}

void Mesh::AddMaterial(Ref<Material> material) {
    assert(material.IsNotNull());
    mMaterials.push_back(std::move(material));
}

void Mesh::AddSubMesh(const StringName &name, RHIPrimitivesType primitivesType, const Aabbf &aabb, uint32 baseVertex, RHIIndexType indexType, uint32 indicesCount, const Ref<Data> &indexData) {
    assert(indexType != RHIIndexType::Unknown);
    assert(indicesCount > 0);
    assert(indexData.IsNotNull());
    assert(indicesCount * RHIGetIndexSize(indexType) == indexData->GetSize());

    auto query = mSubMeshLookUp.find(name);

    if (query != mSubMeshLookUp.end()) {
        BRK_ERROR("Attempt to sub-mesh with the same name sub-mesh=" << name << " inside mesh=" << GetName());
        return;
    }

    auto meshIndex = static_cast<uint32>(mSubMeshes.size());
    Ref<SubMesh> subMesh(new SubMesh);

    subMesh->mName = name;
    subMesh->mPrimitivesType = primitivesType;
    subMesh->mAabb = aabb;
    subMesh->mBaseVertex = baseVertex;
    subMesh->mIndexType = indexType;
    subMesh->mIndicesCount = indicesCount;

    auto &device = Engine::Instance().GetRHIDevice();

    RHIBufferDesc bufferDesc{};
    bufferDesc.size = subMesh->mIndicesCount * RHIGetIndexSize(subMesh->mIndexType);
    bufferDesc.bufferUsage = RHIBufferUsage::Static;

    subMesh->mIndexBuffer = device.CreateIndexBuffer(bufferDesc);
    device.UpdateIndexBuffer(subMesh->mIndexBuffer, 0, bufferDesc.size, indexData);

    mSubMeshes.push_back(std::move(subMesh));
    mSubMeshLookUp.emplace(name, meshIndex);
}

void Mesh::SetSubMeshMaterial(const StringName &name, uint32 index) {
    if (index >= mMaterials.size()) {
        BRK_ERROR("Material index is out of bound for sub-mesh=" << name << " inside mesh=" << GetName());
        return;
    }

    auto query = mSubMeshLookUp.find(name);

    if (query == mSubMeshLookUp.end()) {
        BRK_ERROR("Failed to find sub-mesh name=" << name << " inside mesh=" << GetName());
        return;
    }

    mSubMeshes[query->second]->mMaterialIndex = index;
}

Ref<const SubMesh> Mesh::FindSubMesh(const StringName &name) const {
    auto query = mSubMeshLookUp.find(name);
    return query != mSubMeshLookUp.end() ? mSubMeshes[query->second].As<const SubMesh>() : Ref<const SubMesh>();
}

void Mesh::InitFromData(MeshFormat format, uint32 verticesCount, const Ref<Data> &vertexData, const Ref<Data> &attributeData, const Ref<Data> &skinningData) {
    mFormat = format;
    mVerticesCount = verticesCount;

    uint32 vertexStride = 0;
    uint32 attributeStride = 0;
    uint32 skinningStride = 0;

    auto &engine = Engine::Instance();
    auto &formats = engine.GetRenderEngine().GetMeshFormats();

    formats.GetStride(format, vertexStride, attributeStride, skinningStride);

    assert(!vertexStride || (vertexData.IsNotNull() && vertexData->GetSize() == mVerticesCount * vertexStride));
    assert(!attributeStride || (attributeData.IsNotNull() && attributeData->GetSize() == mVerticesCount * attributeStride));
    assert(!skinningStride || (skinningData.IsNotNull() && skinningData->GetSize() == mVerticesCount * skinningStride));

    auto &device = engine.GetRHIDevice();

    if (vertexData.IsNotNull()) {
        RHIBufferDesc bufferDesc{};
        bufferDesc.bufferUsage = RHIBufferUsage::Static;
        bufferDesc.size = mVerticesCount * vertexStride;
        mVertexData = device.CreateVertexBuffer(bufferDesc);
        device.UpdateVertexBuffer(mVertexData, 0, bufferDesc.size, vertexData);
    }
    if (attributeData.IsNotNull()) {
        RHIBufferDesc bufferDesc{};
        bufferDesc.bufferUsage = RHIBufferUsage::Static;
        bufferDesc.size = mVerticesCount * attributeStride;
        mAttributeData = device.CreateVertexBuffer(bufferDesc);
        device.UpdateVertexBuffer(mAttributeData, 0, bufferDesc.size, attributeData);
    }
    if (skinningData.IsNotNull()) {
        RHIBufferDesc bufferDesc{};
        bufferDesc.bufferUsage = RHIBufferUsage::Static;
        bufferDesc.size = mVerticesCount * skinningStride;
        mSkinningData = device.CreateVertexBuffer(bufferDesc);
        device.UpdateVertexBuffer(mSkinningData, 0, bufferDesc.size, skinningData);
    }

    mDeclaration = std::move(formats.GetDeclaration(mFormat));
}

BRK_NS_END
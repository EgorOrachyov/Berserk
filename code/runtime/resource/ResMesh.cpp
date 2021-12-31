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

#include <resource/ResMesh.hpp>
#include <utility>

BRK_NS_BEGIN

const StringName &ResMesh::GetResourceType() const {
    return GetResourceTypeStatic();
}

const StringName &ResMesh::GetResourceTypeStatic() {
    static StringName resourceType(BRK_TEXT("_brk_resource_mesh"));
    return resourceType;
}

void ResMesh::CreateFromData(MeshFormat format, uint32 verticesCount, const Ref<Data> &vertexData, const Ref<Data> &attributeData, const Ref<Data> &skinningData) {
    mMesh.Reset();
    mSubMeshes.clear();
    mMesh = Ref<Mesh>(new Mesh(format, verticesCount, vertexData, attributeData, skinningData));
    mMesh->SetName(GetName());
}

void ResMesh::AddSubMesh(const StringName &name, RHIPrimitivesType primitivesType, const Aabbf& aabb, uint32 baseVertex, RHIIndexType indexType, uint32 indicesCount, const Ref<Data> &indexData) {
    if (mMesh.IsNull()) {
        BRK_ERROR("No mesh object created in mesh " << GetName());
        return;
    }

    mMesh->AddSubMesh(name, primitivesType, aabb, baseVertex, indexType, indicesCount, indexData);
    mSubMeshes.emplace_back();

    auto& subMesh = mSubMeshes.back();
    subMesh.baseVertex = baseVertex;
    subMesh.indicesCount = indicesCount;
    subMesh.indexType = indexType;
    subMesh.primitivesType = primitivesType;
    subMesh.aabb = aabb;
}

BRK_NS_END
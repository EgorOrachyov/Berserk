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

#ifndef BERSERK_MESH_HPP
#define BERSERK_MESH_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/math/TAabb.hpp>
#include <core/templates/RefCnt.hpp>

#include <render/material/Material.hpp>
#include <render/mesh/MeshFormats.hpp>

#include <rhi/RHIBuffer.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

#include <unordered_map>
#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class SubMesh
 * @brief Separate sufficient entity inside mesh object
 */
class SubMesh final : public RefCnt {
public:
    BRK_API SubMesh() = default;
    BRK_API ~SubMesh() override = default;

    /** @return Name of the sub-mesh */
    BRK_API const StringName &GetName() const { return mName; }

    /** @return Index buffer of the sub-mesh */
    BRK_API const Ref<RHIIndexBuffer> &GetIndexBuffer() const { return mIndexBuffer; }
    /** @return Type of primitives */
    BRK_API RHIPrimitivesType GetPrimitivesType() const { return mPrimitivesType; }
    /** @return Type of indices if present */
    BRK_API RHIIndexType GetIndexType() const { return mIndexType; }
    /** @return Bounding box of sub-mesh */
    BRK_API const Aabbf &GetAabb() const { return mAabb; }

    /** @return Number of vertices to draw [for vertex sub-mesh] */
    BRK_API uint32 GetVerticesCount() const { return mVerticesCount; }
    /** @return Base vertex to draw [for vertex/index sub-mesh] */
    BRK_API uint32 GetBaseVertex() const { return mBaseVertex; }
    /** @return Indices to draw [for index sub-mesh] */
    BRK_API uint32 GetIndicesCount() const { return mIndicesCount; }
    /** @return Index of the material in the mesh */
    BRK_API uint32 GetMaterialIndex() const { return mMaterialIndex; }

    /** @return True if sub-mesh is indexed and uses index buffer for rendering*/
    BRK_API bool IsIndexed() const { return mIndexType != RHIIndexType::Unknown; }

private:
    friend class Mesh;

    StringName mName; /** Name of the sub-mesh */

    Ref<RHIIndexBuffer> mIndexBuffer;                                 /** Separate indices buffer of the sub-mesh */
    RHIPrimitivesType mPrimitivesType = RHIPrimitivesType::Triangles; /** Type of primitives */
    RHIIndexType mIndexType = RHIIndexType::Unknown;                  /** Indices type (if no indices, unknown) */
    Aabbf mAabb;                                                      /** Bounding box of sub-mesh */

    uint32 mVerticesCount{};            /** [vertex mesh] vertices to draw */
    uint32 mBaseVertex{};               /** [vertex/index mesh] base vertex */
    uint32 mIndicesCount{};             /** [index mesh] index to draw */
    uint32 mMaterialIndex = 0xffffffff; /** Index of the material in the mesh */
};

/**
 * @class Mesh
 * @brief Contains vertex array-based geometry
 *
 * Mesh is a type of rendering resource that contains vertex array-based geometry,
 * divided into a set of sub-meshes. Each sub-mesh has separate list of
 * index data, can use separate material and primitives type
 * for rendering.
 *
 * @see Material
 * @see MeshFormat
 * @see MeshFormats
 * @see MeshAttribute
 */
class Mesh final : public RefCnt {
public:
    /**
     * @brief Creates mesh from raw vertex data
     *
     * @param format Format of the vertex
     * @param verticesCount Number of vertices in the mesh
     * @param vertexData Packed vertex data
     * @param attributeData Packed attribute data
     * @param skinningData Packed skinning data
     */
    BRK_API Mesh(MeshFormat format, uint32 verticesCount, const Ref<Data> &vertexData, const Ref<Data> &attributeData, const Ref<Data> &skinningData);
    BRK_API ~Mesh() override = default;

    /** Set name of the mesh */
    BRK_API void SetName(StringName name);
    /** Set mesh bounding box */
    BRK_API void SetAabb(const Aabbf &aabb);

    /** Adds new material to the mesh */
    BRK_API void AddMaterial(Ref<Material> material);

    /**
     * @brief Add new indexed sub-mesh to the mesh
     *
     * @param name Unique name of the sub-mesh
     * @param primitivesType Type of sub-mesh primitives
     * @param aabb Bounding box of sub-mesh
     * @param baseVertex base vertex index
     * @param indexType Type of indices
     * @param indicesCount Number of indices
     * @param indexData Index data
     */
    BRK_API void AddSubMesh(const StringName &name, RHIPrimitivesType primitivesType, const Aabbf& aabb, uint32 baseVertex, RHIIndexType indexType, uint32 indicesCount, const Ref<Data> &indexData);

    /**
     * @brief Set sub-mesh material
     *
     * @param name Name of the sub-mesh
     * @param index Index of the material in the mesh materials list
     */
    BRK_API void SetSubMeshMaterial(const StringName &name, uint32 index);

    /** @return Sub-mesh found by name; null if not found */
    BRK_API Ref<const SubMesh> FindSubMesh(const StringName &name) const;

    /** @return Name of the mesh */
    BRK_API const StringName &GetName() const { return mName; }
    /** @return Format of the mesh */
    BRK_API const MeshFormat &GetFormat() const { return mFormat; }
    /** @return Bounding box of mesh */
    BRK_API const Aabbf &GetAabb() const { return mAabb; }

    /** @return List of materials used by the mesh */
    BRK_API const std::vector<Ref<Material>> &GetMaterials() const { return mMaterials; }
    /** @return List of sub-meshes */
    BRK_API const std::vector<Ref<SubMesh>> &GetSubMeshes() const { return mSubMeshes; }
    /** @return Name to id resoling */
    BRK_API const std::unordered_map<StringName, uint32> &GetSubMeshLookUp() const { return mSubMeshLookUp; }

    /** Number of vertices in the mesh */
    BRK_API uint32 GetVerticesCount() const { return mVerticesCount; }

    /** pos,norm,tan data of the mesh */
    BRK_API const Ref<RHIVertexBuffer> &GetVertexData() const { return mVertexData; }
    /** color,uv's data of the mesh */
    BRK_API const Ref<RHIVertexBuffer> &GetAttributeData() const { return mAttributeData; }
    /** bones, weights data of the mesh */
    BRK_API const Ref<RHIVertexBuffer> &GetSkinningData() const { return mSkinningData; }

    /** Vertex declaration for rendering with full data */
    BRK_API const Ref<RHIVertexDeclaration> &GetDeclaration() const { return mDeclaration; }

    BRK_API bool HasVertexData() const { return mVertexData.IsNotNull(); }
    BRK_API bool HasAttributeData() const { return mAttributeData.IsNotNull(); }
    BRK_API bool HasSkinningData() const { return mSkinningData.IsNotNull(); }

private:
    StringName mName;   /** Name of the mesh */
    MeshFormat mFormat; /** Format of the mesh */
    Aabbf mAabb;        /** Bounding box of mesh */

    std::vector<Ref<Material>> mMaterials;                 /** List of materials used by the mesh */
    std::vector<Ref<SubMesh>> mSubMeshes;                  /** List of sub-meshes */
    std::unordered_map<StringName, uint32> mSubMeshLookUp; /** Name to id resoling */

    uint32 mVerticesCount = 0; /** Number of vertices in the mesh */

    Ref<RHIVertexBuffer> mVertexData;    /** pos,norm,tan data of the mesh */
    Ref<RHIVertexBuffer> mAttributeData; /** color,uv's data of the mesh */
    Ref<RHIVertexBuffer> mSkinningData;  /** bones, weights data of the mesh */

    Ref<RHIVertexDeclaration> mDeclaration; /** Vertex declaration for rendering with full data */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_MESH_HPP

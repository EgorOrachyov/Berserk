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

#ifndef BERSERK_RESMESH_HPP
#define BERSERK_RESMESH_HPP

#include <resource/ResMaterial.hpp>
#include <resource/ResTexture.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>

#include <render/mesh/Mesh.hpp>
#include <render/mesh/MeshFormats.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ResMeshImportOptions
 * @brief Texture import options
 */
class ResMeshImportOptions : public ResourceImportOptions {
public:
    BRK_API ResMeshImportOptions() = default;
    BRK_API ~ResMeshImportOptions() override = default;

    MeshFormat meshFormat = {MeshAttribute::Position, MeshAttribute::Normal, MeshAttribute::Tangent, MeshAttribute::UV}; /** Format of the data to import and preserve */

    bool flipUVs = true;     /** Flip uv coords on loading */
    bool triangulate = true; /** Triangulate so primitives type is triangles */
    bool indexed = true;     /** Make indices to draw indexed */
};

/**
 * @class ResMesh
 * @brief 3d mesh geometry for rendering
 */
class ResMesh final : public Resource {
public:
    /** @brief Single sub-mesh info */
    struct SubMesh {
        uint32 baseVertex{};
        uint32 verticesCount{};
        uint32 indicesCount{};
        RHIIndexType indexType;
        RHIPrimitivesType primitivesType;
        Ref<ResMaterial> material;
        Aabbf aabb;
    };

    BRK_API ResMesh() = default;
    BRK_API ~ResMesh() override = default;

    BRK_API const StringName &GetResourceType() const override;
    BRK_API static const StringName &GetResourceTypeStatic();

    BRK_API void CreateFromArrays(MeshFormat format, uint32 verticesCount, const MeshArrays &arrays);
    BRK_API void CreateFromData(MeshFormat format, uint32 verticesCount, const Ref<Data> &vertexData, const Ref<Data> &attributeData, const Ref<Data> &skinningData);
    BRK_API void AddSubMesh(const StringName &name, RHIPrimitivesType primitivesType, const Aabbf &aabb, uint32 baseVertex, RHIIndexType indexType, uint32 indicesCount, const Ref<Data> &indexData);
    BRK_API void SetAabb(const Aabbf &aabb);

    BRK_API const Ref<Mesh> &GetMesh() const { return mMesh; }
    BRK_API const std::vector<SubMesh> &GetSubMeshes() const { return mSubMeshes; }

private:
    Ref<Mesh> mMesh;                 /** Internal mesh resource for rendering */
    std::vector<SubMesh> mSubMeshes; /** Cached light sub-meshes info */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESMESH_HPP

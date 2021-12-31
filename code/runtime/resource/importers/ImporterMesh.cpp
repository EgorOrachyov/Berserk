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
#include <core/math/Geometry.hpp>

#include <resource/ResMesh.hpp>
#include <resource/importers/ImporterMesh.hpp>

#include <array>
#include <vector>

#include <tiny_obj_loader.h>

BRK_NS_BEGIN

ImporterMesh::ImporterMesh() {
    mExtensions.emplace_back("obj");
}

Ref<ResourceImportOptions> ImporterMesh::CreateDefaultOptions() const {
    return Ref<ResourceImportOptions>(new ResMeshImportOptions);
}

const std::vector<String> &ImporterMesh::GetSupportedExtensions() const {
    return mExtensions;
}

void ImporterMesh::Import(const String &fullpath, const Ref<ResourceImportOptions> &options, ResourceImportResult &result) {
    auto opt = options.Cast<ResMeshImportOptions>();

    if (opt.IsNull()) {
        BRK_ERROR("Passed invalid options for import file=" << fullpath);
        opt = Ref<ResMeshImportOptions>(new ResMeshImportOptions);
    }

    using namespace tinyobj;

    // Import params
    auto pMeshFormat = opt->meshFormat;
    auto pTriangulate = opt->triangulate;
    auto pFlipUVs = opt->flipUVs;
    auto pIndexed = opt->indexed;
    auto pFallbackColors = pMeshFormat.Get(MeshAttribute::Color);

    assert(pTriangulate);
    assert(pIndexed);

    // Imported data
    attrib_t attrib;
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;
    std::string warning;
    std::string error;

    // Actual import
    auto success = LoadObj(&attrib, &shapes, &materials, &warning, &error, fullpath.data(), nullptr, pTriangulate, pFallbackColors);

    // Always display warning if present
    if (!warning.empty()) {
        BRK_WARNING("Warning: \"" << warning << "\" on import file=" << fullpath);
    }

    // Check success first
    if (!success) {
        result.failed = true;
        result.error = std::move(error);
        return;
    }

    // Check which attributes we have
    auto hasPosition = !attrib.vertices.empty();
    auto hasNormal = !attrib.normals.empty() && pMeshFormat.Get(MeshAttribute::Normal);
    auto hasUV = !attrib.texcoords.empty() && pMeshFormat.Get(MeshAttribute::UV);
    auto hasColor = !attrib.colors.empty() && pMeshFormat.Get(MeshAttribute::Color);
    auto hasTangent = hasPosition && hasNormal && hasUV && pMeshFormat.Get(MeshAttribute::Tangent);

    // Position is mandatory
    if (!hasPosition) {
        result.failed = true;
        result.error = BRK_TEXT("No position data in the file");
        return;
    }

    MeshFormat actualFormat;
    actualFormat.Set(MeshAttribute::Position, hasPosition);
    actualFormat.Set(MeshAttribute::Normal, hasNormal);
    actualFormat.Set(MeshAttribute::UV, hasUV);
    actualFormat.Set(MeshAttribute::Color, hasColor);
    actualFormat.Set(MeshAttribute::Tangent, hasTangent);

    static const uint32 VERTICES_PER_FACE = 3;// handle triangles only

    // FLip uvs
    if (pFlipUVs) {
        for (size_t i = 1; i < attrib.texcoords.size(); i += 2) {
            attrib.texcoords[i] = 1.0f - attrib.texcoords[i];
        }
    }

    // Generate tangents
    std::vector<Vec3f> packedTangents;
    if (hasTangent) {
        for (auto &shape : shapes) {
            auto &mesh = shape.mesh;
            auto &indices = mesh.indices;

            for (uint32 i = 0; i < indices.size(); i += VERTICES_PER_FACE) {
                // To generate tangent vectors
                std::array<Vec3f, VERTICES_PER_FACE> positionsPerFace;
                std::array<Vec3f, VERTICES_PER_FACE> normalsPerFace;
                std::array<Vec2f, VERTICES_PER_FACE> texCoordsPerFace;

                for (uint32 v = 0; v < VERTICES_PER_FACE; v++) {
                    auto posX = attrib.vertices[3 * indices[i + v].vertex_index + 0];
                    auto posY = attrib.vertices[3 * indices[i + v].vertex_index + 1];
                    auto posZ = attrib.vertices[3 * indices[i + v].vertex_index + 2];

                    positionsPerFace[v] = Vec3f(posX, posY, posZ);

                    auto normX = attrib.normals[3 * indices[i + v].normal_index + 0];
                    auto normY = attrib.normals[3 * indices[i + v].normal_index + 1];
                    auto normZ = attrib.normals[3 * indices[i + v].normal_index + 2];

                    normalsPerFace[v] = Vec3f(normX, normY, normZ).Normalized();

                    auto texCoordsU = attrib.texcoords[2 * indices[i + v].texcoord_index + 0];
                    auto texCoordsV = attrib.texcoords[2 * indices[i + v].texcoord_index + 1];

                    texCoordsPerFace[v] = Vec2f(texCoordsU, texCoordsV);
                }

                std::array<Vec3f, VERTICES_PER_FACE> tans;
                std::array<Vec3f, VERTICES_PER_FACE> bitans;

                Geometry::GenTangentSpace(positionsPerFace, normalsPerFace, texCoordsPerFace, tans, bitans);

                for (uint32 v = 0; v < VERTICES_PER_FACE; v++) {
                    packedTangents.push_back(tans[v]);
                }
            }
        }
    }

    std::vector<Vec3f> packedPositions;
    std::vector<Vec3f> packedNormals;
    std::vector<Vec3f> packedColors;
    std::vector<Vec2f> packedUVs;

    for (auto &shape : shapes) {
        auto &mesh = shape.mesh;
        auto &indices = mesh.indices;

        for (auto &index : indices) {
            // Positions
            {
                uint32 idx = index.vertex_index;
                auto posX = attrib.vertices[3 * idx + 0];
                auto posY = attrib.vertices[3 * idx + 1];
                auto posZ = attrib.vertices[3 * idx + 2];
                packedPositions.emplace_back(posX, posY, posZ);
            }

            if (hasNormal) {
                uint32 idx = index.normal_index;
                auto normX = attrib.normals[3 * idx + 0];
                auto normY = attrib.normals[3 * idx + 1];
                auto normZ = attrib.normals[3 * idx + 2];
                packedNormals.push_back(Vec3f(normX, normY, normZ).Normalized());
            }
            if (hasColor) {
                uint32 idx = index.vertex_index;
                auto colR = attrib.colors[3 * idx + 0];
                auto colG = attrib.colors[3 * idx + 1];
                auto colB = attrib.colors[3 * idx + 2];
                packedColors.emplace_back(colR, colG, colB);
            }
            if (hasUV) {
                uint32 idx = index.texcoord_index;
                auto texCoordsU = attrib.texcoords[2 * idx + 0];
                auto texCoordsV = attrib.texcoords[2 * idx + 1];
                packedUVs.emplace_back(texCoordsU, texCoordsV);
            }
        }
    }

    // Fill arrays with data
    MeshArrays arrays;
    arrays.positions = reinterpret_cast<const float *>(packedPositions.data());
    if (hasNormal) arrays.normals = reinterpret_cast<const float *>(packedNormals.data());
    if (hasTangent) arrays.tangents = reinterpret_cast<const float *>(packedTangents.data());
    if (hasColor) arrays.colors = reinterpret_cast<const float *>(packedColors.data());
    if (hasUV) arrays.uvs = reinterpret_cast<const float *>(packedUVs.data());
    auto verticesCount = static_cast<uint32>(packedPositions.size());

    // Create resource mesh from arrays
    Ref<ResMesh> resMesh(new ResMesh);
    resMesh->SetName(StringName(Engine::Instance().GetFileSystem().GetFileName(fullpath, true)));
    resMesh->CreateFromArrays(actualFormat, verticesCount, arrays);

    // Create sub-meshes from shapes
    Aabbf meshAabb;
    std::size_t baseIndex = 0;

    for (auto &shape : shapes) {
        auto &mesh = shape.mesh;
        auto &indices = mesh.indices;

        Aabbf shapeAabb;
        StringName shapeName(shape.name);
        RHIPrimitivesType primitivesType = RHIPrimitivesType::Triangles;
        RHIIndexType indexType = RHIIndexType::Uint32;

        std::size_t nextIndex = 0;
        std::size_t indexSize = RHIGetIndexSize(indexType);
        std::size_t indicesCount = indices.size();
        Ref<Data> indicesData = Data::Make(indexSize * indicesCount);
        auto *indicesGenerated = reinterpret_cast<uint32 *>(indicesData->GetDataWrite());

        for (std::size_t i = 0; i < indicesCount; i++) {
            uint32 idx = indices[i].vertex_index;
            auto posX = attrib.vertices[3 * idx + 0];
            auto posY = attrib.vertices[3 * idx + 1];
            auto posZ = attrib.vertices[3 * idx + 2];
            auto attribute = Vec3f(posX, posY, posZ);
            shapeAabb.Fit(attribute);
            indicesGenerated[nextIndex++] = static_cast<uint32>(baseIndex + i);
        }

        meshAabb.Fit(shapeAabb);
        resMesh->AddSubMesh(shapeName, primitivesType, shapeAabb, 0, indexType, static_cast<uint32>(indicesCount), indicesData);

        baseIndex += nextIndex;
    }

    resMesh->SetAabb(meshAabb);
    result.resource = resMesh.As<Resource>();
}

BRK_NS_END
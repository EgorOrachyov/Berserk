/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <MeshImporter/MeshImporter.h>
#include <LogMacro.h>
#include <Math/MeshUtility.h>
#include <Resources/Mesh.h>
#include <Resources/MeshImportOptions.h>

namespace Berserk {

    MeshImporter::MeshImporter() {
        mRecognizedExtensions.ensureToAdd(1);
        mRecognizedExtensions.emplace("obj");

        registerImporter();
        BERSERK_LOG_INFO("Initialize MeshImporter (obj)");
    }

    MeshImporter::~MeshImporter() {
        unregisteImporter();
        BERSERK_LOG_INFO("Finalize MeshImporter (obj)");
    }

    EError MeshImporter::import(TPtrShared<Resource> &resource, const CString &importPath, TRef<const ResourceImportOptions> options) {
        Guard scopedLock(mAccessMutex);
        using namespace tinyobj;

        const MeshImportOptions* meshImportOptions = &Mesh::getDefaultImportOptions();

        if (options.isNotNull()) {
            auto optionsPtr = options.getPtr();
            auto imageOptionsPtr = dynamic_cast<const MeshImportOptions*>(optionsPtr);

            BERSERK_COND_ERROR(imageOptionsPtr, "Provided invalid import options for mesh: %s", importPath.data());

            if (imageOptionsPtr)
                meshImportOptions = imageOptionsPtr;
        }

        MeshFormat format = meshImportOptions->getFormat();
        Transformf t = meshImportOptions->getTransform();
        bool flipUVs = meshImportOptions->getFlipUVs();

        bool fTriangulate = true;
        bool fFallbackColors = false;
        bool fIndexed = true;

        attrib_t attrib;
        std::vector<shape_t> shapes;
        std::vector<material_t> materials;
        std::string warning;
        std::string error;

        bool success = LoadObj(&attrib, &shapes, &materials, &warning, &error, importPath.data(), nullptr, fTriangulate, fFallbackColors);

        bool aPos = true;   // Always imported
        bool aNorm = format.getFlag(EMeshAttribute::Normal) && (!attrib.normals.empty());
        bool aTexCoords = format.getFlag(EMeshAttribute::TexCoord) && (!attrib.texcoords.empty());
        bool aColor = format.getFlag(EMeshAttribute::Color) && (!attrib.colors.empty());
        bool aTang = format.getFlag(EMeshAttribute::Tangent) && aPos && aNorm && aTexCoords;

        format.setFlag(EMeshAttribute::Position, aPos);
        format.setFlag(EMeshAttribute::Normal, aNorm);
        format.setFlag(EMeshAttribute::TexCoord, aTexCoords);
        format.setFlag(EMeshAttribute::Color, aColor);
        format.setFlag(EMeshAttribute::Tangent, aTang);

        BERSERK_COND_ERROR(error.empty(), "%s", error.c_str());
        BERSERK_COND_WARNING(warning.empty(), "%s", warning.c_str());
        BERSERK_COND_ERROR_RET_VALUE(EError::FAILED_IMPORT_RESOURCE, success, "Failed import mesh: %s", importPath.data());

        mTangents.clear();
        mTangents.resize(attrib.vertices.size());

        if (aTang) {
            for (auto& shape: shapes) {
                auto& mesh = shape.mesh;
                auto& indices = mesh.indices;

                for (uint32 i = 0; i < indices.size(); i += VERTICES_PER_FACE) {
                    // To generate tangent vectors
                    TArrayStatic<Vec3f,VERTICES_PER_FACE> positionsPerFace;
                    TArrayStatic<Vec3f,VERTICES_PER_FACE> normalsPerFace;
                    TArrayStatic<Vec2f,VERTICES_PER_FACE> texCoordsPerFace;

                    for (uint32 v = 0; v < VERTICES_PER_FACE; v++) {
                        auto posX = attrib.vertices[3 * indices[i + v].vertex_index + 0];
                        auto posY = attrib.vertices[3 * indices[i + v].vertex_index + 1];
                        auto posZ = attrib.vertices[3 * indices[i + v].vertex_index + 2];

                        positionsPerFace.add(t.transform(Vec3f(posX, posY, posZ)));

                        auto normX = attrib.normals[3 * indices[i + v].normal_index + 0];
                        auto normY = attrib.normals[3 * indices[i + v].normal_index + 1];
                        auto normZ = attrib.normals[3 * indices[i + v].normal_index + 2];

                        normalsPerFace.add(t.transformAsNormal(Vec3f(normX, normY, normZ)).normalized());

                        auto texCoordsU = attrib.texcoords[2 * indices[i + v].texcoord_index + 0];
                        auto texCoordsV = attrib.texcoords[2 * indices[i + v].texcoord_index + 1];

                        if (flipUVs) {
                            texCoordsV = 1.0f - texCoordsV;
                        }

                        texCoordsPerFace.add(Vec2f(texCoordsU, texCoordsV));
                    }

                    TArrayStatic<Vec3f,VERTICES_PER_FACE> tangents;
                    TArrayStatic<Vec3f,VERTICES_PER_FACE> bitangents;

                    MeshUtility::generateTangentSpace(positionsPerFace, normalsPerFace, texCoordsPerFace, tangents, bitangents);

                    for (uint32 v = 0; v < VERTICES_PER_FACE; v++) {
                        mTangents[indices[i + v].vertex_index] = tangents[v];
                    }
                }
            }
        }

        CString name = importPath.getFileNameFromPath().removeExtension();
        EPrimitivesType primitivesType = EPrimitivesType::Triangles;
        EIndexType indexType = EIndexType ::Uint32;
        BinaryData vertexData;
        BinaryData indexData;
        uint32 verticesCount = attrib.vertices.size() / DIMENSION_3D;
        uint32 indicesCount = 0;
        Aabbf aabbf;
        TArray<MeshNode> meshNodes;

        uint32 stride = MeshFormatUtil::getStrideSizeForFormat(format);
        vertexData.resize(stride * verticesCount);

        for (auto& shape: shapes) {

            Aabbf shapeAabb;
            uint32 shapeIndicesOffset = indicesCount;
            uint32 shapeIndicesCount = shape.mesh.indices.size();

            indicesCount += shapeIndicesCount;

            for (auto& index: shape.mesh.indices) {

                indexData.add((uint32) index.vertex_index);
                auto offset = stride * index.vertex_index;

                if (aPos) {
                    uint32 idx = index.vertex_index;

                    auto posX = attrib.vertices[DIMENSION_3D * idx + 0];
                    auto posY = attrib.vertices[DIMENSION_3D * idx + 1];
                    auto posZ = attrib.vertices[DIMENSION_3D * idx + 2];

                    auto attribute = t.transform(Vec3f(posX, posY, posZ));

                    shapeAabb.fit(attribute);

                    vertexData.write(offset, (uint8*)&attribute, sizeof(attribute));
                    offset += sizeof(attribute);
                }

                if (aNorm) {
                    uint32 idx = index.normal_index;

                    auto normX = attrib.normals[DIMENSION_3D * idx + 0];
                    auto normY = attrib.normals[DIMENSION_3D * idx + 1];
                    auto normZ = attrib.normals[DIMENSION_3D * idx + 2];

                    auto attribute = t.transformAsNormal(Vec3f(normX, normY, normZ)).normalized();

                    vertexData.write(offset, (uint8*)&attribute, sizeof(attribute));
                    offset += sizeof(attribute);
                }

                if (aTang) {
                    uint32 idx = index.vertex_index;

                    auto attribute = mTangents[idx];
                    vertexData.write(offset, (uint8*)&attribute, sizeof(attribute));
                    offset += sizeof(attribute);
                }

                if (aColor) {
                    uint32 idx = index.vertex_index;

                    auto colR = attrib.colors[DIMENSION_3D * idx + 0];
                    auto colG = attrib.colors[DIMENSION_3D * idx + 1];
                    auto colB = attrib.colors[DIMENSION_3D * idx + 2];

                    auto attribute = Vec3f(colR, colG, colB);
                    vertexData.write(offset, (uint8*)&attribute, sizeof(attribute));
                    offset += sizeof(attribute);
                }

                if (aTexCoords) {
                    uint32 idx = index.texcoord_index;

                    auto texCoordsU = attrib.texcoords[DIMENSION_2D * idx + 0];
                    auto texCoordsV = attrib.texcoords[DIMENSION_2D * idx + 1];

                    if (flipUVs) {
                        texCoordsV = 1.0f - texCoordsV;
                    }

                    auto attribute = Vec2f(texCoordsU, texCoordsV);
                    vertexData.write(offset, (uint8*)&attribute, sizeof(attribute));

                    offset += sizeof(attribute);
                }

            }

            aabbf.fit(shapeAabb);

            MeshNode meshNode;
            meshNode.nodeName = shape.name.c_str();
            meshNode.mAabb = shapeAabb;
            meshNode.index = meshNodes.size();
            meshNode.indicesCount = shapeIndicesCount;
            meshNode.indicesOffset = shapeIndicesOffset;

            meshNodes.move(meshNode);
        }

#if 1
        printf("Import mesh %s\n", name.data());
        printf(" Format: %s\n", MeshFormatUtil::getFormatAsString(format).data());
        printf(" Vertices: %u Indices: %u\n", verticesCount, indicesCount);
        printf(" Nodes: %u\n", meshNodes.size());
        for (auto& node: meshNodes) {
            printf("  Name: %s, Indices: %u, IndicesOffset: %u\n", node.nodeName.data(), node.indicesCount, node.indicesOffset);
        }
#endif

        TPtrShared<Mesh> mesh = TPtrShared<Mesh>::make(primitivesType, format, aabbf, std::move(meshNodes), verticesCount, indicesCount, vertexData, indexData, indexType);
        mesh->setName(name);
        mesh->setUUID(UUID::generate());
        mesh->setImportPath(importPath);
        mesh->setImportTime(TimeValue::nowAsTime());

        resource = mesh.castTo<Resource>();

        return EError::OK;
    }

    const char* MeshImporter::getImporterName() const {
        return "MeshImporter";
    }

    const char* MeshImporter::getImporterProjectName() {
        return "BerserkImporters";
    }

    const TArray<CString>& MeshImporter::getRecognizedExtensions() const {
        return mRecognizedExtensions;
    }

    bool MeshImporter::isThreadSafe() const {
        return true;
    }

}
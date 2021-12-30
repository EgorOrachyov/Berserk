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
#include <core/io/Logger.hpp>
#include <render/mesh/MeshFormats.hpp>

BRK_NS_BEGIN

static MeshAttribute VERTEX_DATA[] = {MeshAttribute::Position, MeshAttribute::Normal, MeshAttribute::Tangent};
static MeshAttribute ATTRIBUTE_DATA[] = {MeshAttribute::Color, MeshAttribute::UV, MeshAttribute::UV2};
static MeshAttribute SKINNING_DATA[] = {MeshAttribute::Weights, MeshAttribute::Bones};

static uint32 VERTEX_DATA_SIZE = 3;
static uint32 ATTRIBUTE_DATA_SIZE = 3;
static uint32 SKINNING_DATA_SIZE = 2;

Ref<RHIVertexDeclaration> MeshFormats::GetDeclaration(MeshFormat target, MeshFormat format) {
    auto k1 = target.value.to_string();
    auto k2 = format.value.to_string();

    auto key = k1;
    key += "-";
    key += k2;

    auto query = mCached.find(key);

    if (query != mCached.end()) {
        return query->second;
    }

    if ((target.value & format.value) != target.value) {
        BRK_ERROR("Invalid MeshFormat target=" << k1 << " passed when format=" << k2);
        return Ref<RHIVertexDeclaration>();
    }

    RHIVertexDeclarationDesc desc;

    uint8 buffer = 0;

    auto setupData = [&](MeshAttribute *attributes, uint32 attributesCount) {
        uint32 offset = 0;
        size_t first = desc.size();
        bool hasAttribute = false;

        for (uint32 i = 0; i < attributesCount; i++) {
            auto attrib = attributes[i];

            // Only if attribute in source format
            if (format.Get(attrib)) {
                auto info = MeshGetAttributeInfo(attrib);

                // If attribute in target format, add it
                if (target.Get(attrib)) {
                    desc.emplace_back();
                    auto &entry = desc.back();
                    entry.offset = offset;
                    entry.type = info.elementType;
                    entry.buffer = buffer;
                    entry.frequency = RHIVertexFrequency::PerVertex;
                    hasAttribute = true;
                }

                offset += info.size;
            }
        }

        // Fix stride
        for (size_t i = first; i < desc.size(); i++) {
            desc[i].stride = offset;
        }

        // Update buffer index
        if (hasAttribute)
            buffer += 1;
    };

    setupData(VERTEX_DATA, VERTEX_DATA_SIZE);
    setupData(ATTRIBUTE_DATA, ATTRIBUTE_DATA_SIZE);
    setupData(SKINNING_DATA, SKINNING_DATA_SIZE);

    auto &device = Engine::Instance().GetRHIDevice();
    auto declaration = device.CreateVertexDeclaration(desc);

#ifdef BERSERK_DEBUG
    BRK_INFO("Create mesh vertex declaration key=" << key);
#endif//BERSERK_DEBUG

    mCached.emplace(std::move(key), declaration);
    return declaration;
}

Ref<RHIVertexDeclaration> MeshFormats::GetDeclaration(MeshFormat target) {
    return GetDeclaration(target, target);
}

uint32 MeshFormats::GetAttributeOffset(MeshAttribute attribute, MeshFormat format) {
    MeshAttributeInfo info = MeshGetAttributeInfo(attribute);
    uint32 offset = 0;

    auto checkData = [&](bool check, MeshAttribute *attributes, uint32 attributesCount) {
        if (check) {
            uint32 current = 0;

            while (attribute != attributes[current] && current < attributesCount) {
                if (format.Get(attributes[current]))
                    offset += MeshGetAttributeInfo(attributes[current]).size;
                current += 1;
            }
        }
    };

    checkData(info.vertex, VERTEX_DATA, VERTEX_DATA_SIZE);
    checkData(info.attribute, ATTRIBUTE_DATA, ATTRIBUTE_DATA_SIZE);
    checkData(info.skinning, SKINNING_DATA, SKINNING_DATA_SIZE);

    return offset;
}

void MeshFormats::GetStride(MeshFormat format, uint32 &vertex, uint32 &attribute, uint32 &skinning) {
    vertex = attribute = skinning = 0u;

    auto defineStride = [&](uint32 &s, MeshAttribute *attributes, uint32 attributesCount) {
        for (uint32 i = 0; i < attributesCount; i++)
            if (format.Get(attributes[i]))
                s += MeshGetAttributeInfo(attributes[i]).size;
    };

    defineStride(vertex, VERTEX_DATA, VERTEX_DATA_SIZE);
    defineStride(attribute, ATTRIBUTE_DATA, ATTRIBUTE_DATA_SIZE);
    defineStride(skinning, SKINNING_DATA, SKINNING_DATA_SIZE);
}

BRK_NS_END
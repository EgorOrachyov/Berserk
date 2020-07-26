/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXARRAYDATA_H
#define BERSERK_VERTEXARRAYDATA_H

#include <VertexStreams.h>
#include <RenderResources/VertexDeclaration.h>

namespace Berserk {
    namespace Render {

        /** Provides stream style access to vertex elements */
        class VertexArrayData {
        public:

            VertexArrayData& setDeclaration(const TPtrShared<VertexDeclaration> &declaration) {
                mDeclaration = declaration;
                mVertexData.resize(declaration->getBuffersCount());
                return *this;
            }

            VertexArrayData& useIndices(EIndexType type = EIndexType::Uint32) {
                mIndexType = type;
                mUseIndices = true;
                return *this;
            }

            ElementStream getStreamFor(const CString& elementName) {
                if (!mDeclaration->hasElement(elementName)) return {};
                auto& element = mDeclaration->getElement(elementName);
                return {element.type, mVertexData[element.buffer], element.offset, element.stride};
            }

            IndexStream getIndexStream() {
                if (!mUseIndices) return {};
                return {mIndexType,mIndexData};
            }

            VertexArrayData& evaluateIndicesCount() {
                if (mUseIndices) {
                    auto indexSize = RHIDefinitionsUtil::getIndexSize(mIndexType);
                    auto indexBufferSize = mIndexData.size();
                    mIndicesCount = indexBufferSize / indexSize;
                }
                return *this;
            }

            VertexArrayData& evaluateVerticesCount() {
                auto& buffers = mDeclaration->getBuffers();

                bool firstEval = true;

                for (uint32 i = 0; i < buffers.size(); i++) {
                    auto& desc = buffers[i];
                    auto& data = mVertexData[i];

                    if (desc.iterating == EVertexIterating::PerInstance)
                        continue;

                    if (data.size() > 0) {
                        auto vertices = data.size() / desc.stride;

                        if (firstEval) {
                            firstEval = false;
                            mVerticesCount = vertices;
                            continue;
                        }

                        BERSERK_COND_ERROR_RET_VALUE(*this,vertices == mVerticesCount, "Vertices count not the same at data buffers");
                    }
                }
                return *this;
            }

            VertexArrayData& evaluateInstancesCount() {
                auto& buffers = mDeclaration->getBuffers();

                bool firstEval = true;

                for (uint32 i = 0; i < buffers.size(); i++) {
                    auto& desc = buffers[i];
                    auto& data = mVertexData[i];

                    if (desc.iterating == EVertexIterating::PerVertex)
                        continue;

                    if (data.size() > 0) {
                        auto instances = data.size() / desc.stride;

                        if (firstEval) {
                            firstEval = false;
                            mInstancesCount = instances;
                            continue;
                        }

                        BERSERK_COND_ERROR_RET_VALUE(*this,instances == mInstancesCount, "Instances count not the same at data buffers");
                    }
                }
                return *this;
            }

            VertexArrayData& evaluate() {
                evaluateIndicesCount();
                evaluateVerticesCount();
                evaluateInstancesCount();
                return *this;
            }

            bool hasIndices() const { return mUseIndices; }

            uint32 getVerticesCount() const { return mVerticesCount; }

            uint32 getIndicesCount() const { return mIndicesCount; }

            uint32 getInstancesCount() const { return mInstancesCount; }

            EIndexType getIndicesType() const { return mIndexType; }

            const TArrayStatic<BinaryData>& getVertexData() const { return mVertexData; }

            const BinaryData& getIndicesData() const { return mIndexData; }

            const TPtrShared<VertexDeclaration> &getDeclaration() const { return mDeclaration; }

        private:
            bool mUseIndices = false;
            uint32 mIndicesCount = 0;
            uint32 mVerticesCount = 0;
            uint32 mInstancesCount = 1;
            EIndexType mIndexType = EIndexType::Unknown;
            TArrayStatic<BinaryData> mVertexData;
            BinaryData mIndexData;
            TPtrShared<VertexDeclaration> mDeclaration;

        };

    }
}




#endif //BERSERK_VERTEXARRAYDATA_H

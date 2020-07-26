/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/VertexArrayUpdate.h>

namespace Berserk {
    namespace Render {

        VertexArrayUpdate::VertexArrayUpdate(const TPtrShared<VertexArray> &array) {
            BERSERK_COND_ERROR_RET(array.isNotNull(), "Null array object");
            BERSERK_COND_ERROR_RET(array->isInitialized(), "Null array object");
            mArray = array;
            mBinaryData.resize(mArray->getVertexBuffers().size());

        }

        ElementStream VertexArrayUpdate::getStreamFor(const CString &elementName) {
            BERSERK_COND_ERROR_RET_VALUE({}, mArray.isNotNull(), "Null array object");

            auto& decl = mArray->getDeclaration();

            if (decl->hasElement(elementName)) {
                auto& element = decl->getElement(elementName);
                auto  bufferIdx = element.buffer;
                auto  size = element.stride * mArray->getVerticesCount();

                auto& bufferData = mBinaryData[bufferIdx];
                auto& bufferRHI = mArray->getVertexBuffers()[bufferIdx];

                bufferData.resize(size);

                return {element.type, bufferRHI.getPtr(), bufferData, element.offset, element.stride, size};
            }

            return {};
        }

        IndexStream VertexArrayUpdate::getIndexStream() {
            BERSERK_COND_ERROR_RET_VALUE({}, mArray.isNotNull(), "Null array object");
            BERSERK_COND_ERROR_RET_VALUE({}, mArray->hasIndices(), "Array object has no indices data");

            auto  type = mArray->getIndicesType();
            auto  range = mArray->getIndicesCount() * RHIDefinitionsUtil::getIndexSize(type);
            auto& bufferRHI = mArray->getIndexBuffer();
            auto& bufferData = mIndexData;

            bufferData.resize(range);

            return {type, bufferRHI.getPtr(), bufferData, range};
        }

        void VertexArrayUpdate::updateDataOnGPU() {
            BERSERK_COND_ERROR_RET(mArray.isNotNull(), "Null array object");

            auto& buffersRHI = mArray->getVertexBuffers();

            for (uint32 i = 0; i < buffersRHI.size(); i++) {
                auto& rhi = buffersRHI[i];
                auto& data = mBinaryData[i];

                if (data.size() > 0) {
                    rhi->update(data.size(), 0, data.data());
                }
            }

            if (mIndexData.size() > 0) {
                auto& indicesRHI = mArray->getIndexBuffer();
                auto  type = mArray->getIndicesType();
                auto  range = mArray->getIndicesCount() * RHIDefinitionsUtil::getIndexSize(type);

                indicesRHI->update(range, 0, mIndexData.data());
            }
        }

    }
}
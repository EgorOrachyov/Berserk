/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <RenderResources/VertexArrayData.h>
#include <RenderResources/VertexArray.h>
#include <RenderResources/VertexArrayBuilder.h>

namespace Berserk {
    namespace Render {

        VertexArrayBuilder& VertexArrayBuilder::configureFromData(const struct VertexArrayData &arrayData) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNull(), "An attempt to reset declaration");
            mDeclaration = arrayData.getDeclaration();
            mVertexBuffers.resize(mDeclaration->getBuffersCount());
            mIndicesType = arrayData.getIndicesType();
            mVerticesCount = arrayData.getVerticesCount();
            mIndicesCount = arrayData.getIndicesCount();
            mInstancesCount = arrayData.getInstancesCount();
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setName(const CString &name) {
            mName = name;
            return *this;
        }

        VertexArrayBuilder & VertexArrayBuilder::setPrimitivesType(EPrimitivesType primitivesType) {
            mPrimitivesType = primitivesType;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setVertexDeclaration(const TPtrShared<VertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET_VALUE(*this, declaration.isNotNull(), "Passed null declaration");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNull(), "An attempt to reset declaration");
            mDeclaration = declaration;
            mVertexBuffers.resize(mDeclaration->getBuffersCount());
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setIndicesCount(uint32 count) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            mIndicesCount = count;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setIndicesType(EIndexType type) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            mIndicesType = type;
            return *this;
        }
        
        VertexArrayBuilder & VertexArrayBuilder::addIndexBuffer(uint32 indicesCount, EIndexType type, const TPtrShared<RHIIndexBuffer> &buffer) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, buffer.isNotNull(), "Passed null buffer");
            mIndicesCount = indicesCount;
            mIndicesType = type;
            mIndexBuffer = buffer;
            return *this;
        }

        VertexArrayBuilder & VertexArrayBuilder::addIndexBuffer(uint32 indicesCount, EIndexType type, EBufferUsage bufferUsage) {
            auto& device = RHIDevice::getSingleton();
            mIndicesCount = indicesCount;
            mIndicesType = type;

            uint32 bufferSize = getIndexBufferSize();
            mIndexBuffer = device.createIndexBuffer(bufferSize, bufferUsage, nullptr);
            return *this;
        }

        VertexArrayBuilder & VertexArrayBuilder::setVerticesCount(uint32 verticesCount) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            mVerticesCount = verticesCount;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::addVertexBuffer(const CString &bufferName, const TPtrShared<RHIVertexBuffer> &buffer) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, buffer.isNotNull(), "Passed null buffer");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration->hasBuffer(bufferName), "No such buffer: %s", bufferName.data());

            auto& b = mDeclaration->getBuffer(bufferName);
            mVertexBuffers[b.index] = buffer;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::addVertexBuffer(const CString &bufferName, EBufferUsage bufferUsage) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration->hasBuffer(bufferName), "No such buffer: %s", bufferName.data());

            auto& device = RHIDevice::getSingleton();
            auto& b = mDeclaration->getBuffer(bufferName);
            uint32 bufferSize = getVertexBufferSize(b);
            auto buffer = device.createVertexBuffer(bufferSize, bufferUsage, nullptr);
            return addVertexBuffer(bufferName, buffer);
        }

        VertexArrayBuilder& VertexArrayBuilder::setInstancesCount(uint32 instancesCount) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, instancesCount > 0, "Instances count must be more than 0");
            mInstancesCount = instancesCount;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setIndexBufferData(const void *data) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, mIndexBuffer.isNotNull(), "No RHI index buffer");

            uint32 bufferSize = getIndexBufferSize();

            mIndexBuffer->update(bufferSize, 0, data);
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setVertexBufferData(const CString& bufferName, const void *data) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration->hasBuffer(bufferName), "No such buffer: %s", bufferName.data());
            BERSERK_COND_ERROR_RET_VALUE(*this, mVertexBuffers[mDeclaration->getBuffer(bufferName).index].isNotNull(), "No RHI buffer for: %s", bufferName.data());

            auto& b = mDeclaration->getBuffer(bufferName);
            uint32 bufferSize = getVertexBufferSize(b);

            mVertexBuffers[b.index]->update(bufferSize, 0, data);
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::allocateBuffers() {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");

            auto& device = RHIDevice::getSingleton();
            auto& buffers = mDeclaration->getBuffers();

            for (uint32 i = 0; i < buffers.size(); i++) {
                if (mVertexBuffers[i].isNull()) {
                    auto& buffer = buffers[i];
                    uint32 bufferSize = getVertexBufferSize(buffer);
                    mVertexBuffers[i] = device.createVertexBuffer(bufferSize, EBufferUsage::Static, nullptr);
                }
            }

            if (mIndicesCount > 0 && mIndexBuffer.isNull()) {
                uint32 bufferSize = getIndexBufferSize();
                mIndexBuffer = device.createIndexBuffer(bufferSize, EBufferUsage::Static, nullptr);
            }

            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setDataFrom(const VertexArrayData &arrayData) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration == arrayData.getDeclaration(), "Vertex declaration must be the same");

            auto& buffers = arrayData.getVertexData();
            auto& indices = arrayData.getIndicesData();

            auto& buffersMeta = mDeclaration->getBuffers();

            for (uint32 i = 0; i < mVertexBuffers.size(); i++) {
                if (buffers[i].size() > 0) {
                    setVertexBufferData(buffersMeta[i].name, buffers[i].data());
                }
            }

            if (arrayData.hasIndices()) {
                if (indices.size() > 0) {
                    setIndexBufferData(indices.data());
                }
            }

            return *this;
        }

        const TPtrShared<class VertexArray>& VertexArrayBuilder::buildShared() {
            if (mInstance.isNotNull())
                return mInstance;

            mInstance = TPtrShared<VertexArray>::make(*this);
            return mInstance;
        }

        uint32 VertexArrayBuilder::getVertexBufferSize(const VertexBufferInfo &info) const {
            return info.stride * (info.iterating == EVertexIterating::PerVertex? mVerticesCount: mInstancesCount);
        }

        uint32 VertexArrayBuilder::getIndexBufferSize()  const {
            return mIndicesCount * RHIDefinitionsUtil::getIndexSize(mIndicesType);
        }
    }

}
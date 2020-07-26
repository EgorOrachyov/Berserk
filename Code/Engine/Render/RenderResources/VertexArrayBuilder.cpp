/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIDevice.h>
#include <RenderResources/VertexArray.h>
#include <RenderResources/VertexArrayBuilder.h>

namespace Berserk {
    namespace Render {

        VertexArrayBuilder& VertexArrayBuilder::setName(const CString &name) {
            mName = name;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setVertexDeclaration(const TPtrShared<VertexDeclaration> &declaration) {
            BERSERK_COND_ERROR_RET_VALUE(*this, declaration.isNotNull(), "Passed null declaration");
            mDeclaration = declaration;
            mVertexBuffers.resize(mDeclaration->getBuffersCount());
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

        VertexArrayBuilder & VertexArrayBuilder::addIndexBuffer(uint32 indicesCount, EIndexType type, EMemoryType memoryType) {
            auto& device = RHIDevice::getSingleton();
            uint32 indexSize = RHIDefinitionsUtil::getIndexSize(type);
            uint32 bufferSize = indicesCount * indexSize;
            auto buffer = device.createIndexBuffer(bufferSize, memoryType, nullptr);
            return addIndexBuffer(indicesCount, type, buffer);
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

        VertexArrayBuilder& VertexArrayBuilder::addVertexBuffer(const CString &bufferName, EMemoryType memoryType) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration->hasBuffer(bufferName), "No such buffer: %s", bufferName.data());

            auto& device = RHIDevice::getSingleton();
            auto& b = mDeclaration->getBuffer(bufferName);
            uint32 bufferSize = b.stride * mVerticesCount;
            auto buffer = device.createVertexBuffer(bufferSize, memoryType, nullptr);
            return addVertexBuffer(bufferName, buffer);
        }

        VertexArrayBuilder& VertexArrayBuilder::setInstancesCount(uint32 instancesCount) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration.isNotNull(), "Vertex declaration is not specified");
            BERSERK_COND_ERROR_RET_VALUE(*this, instancesCount > 0, "Instances count must be more than 0");
            mInstancesCount = instancesCount;
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setIndicesData(const uint32 *data) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mIndexBuffer.isNotNull(), "No RHI index buffer");
            BERSERK_COND_ERROR_RET_VALUE(*this, mIndicesType == EIndexType::Uint32, "Indices has another type");

            uint32 indexSize = RHIDefinitionsUtil::getIndexSize(mIndicesType);
            uint32 bufferSize = mIndicesCount * indexSize;

            mIndexBuffer->update(bufferSize, 0, data);
            return *this;
        }

        VertexArrayBuilder& VertexArrayBuilder::setVertexBufferData(const CString& bufferName, const void *data) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mDeclaration->hasBuffer(bufferName), "No such buffer: %s", bufferName.data());
            BERSERK_COND_ERROR_RET_VALUE(*this, mVertexBuffers[mDeclaration->getBuffer(bufferName).index].isNotNull(), "No RHI buffer for: %s", bufferName.data());

            auto& b = mDeclaration->getBuffer(bufferName);
            uint32 bufferSize = b.stride * mVerticesCount;

            mVertexBuffers[b.index]->update(bufferSize, 0, data);
            return *this;
        }

        const TPtrShared<class VertexArray>& VertexArrayBuilder::buildShared() {
            if (mInstance.isNotNull())
                return mInstance;

            mInstance = TPtrShared<VertexArray>::make(*this);
            return mInstance;
        }
    }

}
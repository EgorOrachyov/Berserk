/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/UniformBufferWriter.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        UniformBufferWriter::UniformBufferWriter(uint32 size) {
            resize(size);
        }

        void UniformBufferWriter::resize(uint32 size) {
            mData.resize(size);
        }

        void UniformBufferWriter::setFloat(float t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(float)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(float));
            mIsDirty = true;
        }

        void UniformBufferWriter::setVec2(const UniformBufferWriter::Vec2 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec2)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, t.getData(), sizeof(Vec2));
            mIsDirty = true;
        }

        void UniformBufferWriter::setVec3(const UniformBufferWriter::Vec3 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec3)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, t.getData(), sizeof(Vec3));
            mIsDirty = true;
        }

        void UniformBufferWriter::setVec4(const UniformBufferWriter::Vec4 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec4)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, t.getData(), sizeof(Vec4));
            mIsDirty = true;
        }

        void UniformBufferWriter::setBool(bool t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Bool)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(Bool));
            mIsDirty = true;
        }

        void UniformBufferWriter::setMat2(const UniformBufferWriter::Mat2 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat2::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat2::dimM(); i++) {
                Memory::copy(d + i * stride, (uint8*) tt.data() + i * Mat2::stride(), Mat2::stride());
            }

            mIsDirty = true;
        }

        void UniformBufferWriter::setMat3(const UniformBufferWriter::Mat3 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat3::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat3::dimM(); i++) {
                Memory::copy(d + i * stride, (uint8*) tt.data() + i * Mat3::stride(), Mat3::stride());
            }

            mIsDirty = true;
        }

        void UniformBufferWriter::setMat4(const UniformBufferWriter::Mat4 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat4::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat4::dimM(); i++) {
                Memory::copy(d + i * stride, (uint8*) tt.data() + i * Mat4::stride(), Mat4::stride());
            }

            mIsDirty = true;
        }
        
        void UniformBufferWriter::updateDataGPU(const TPtrShared<RHIUniformBuffer> &buffer, uint32 offset) {
            BERSERK_COND_ERROR_RET(buffer.isNotNull(), "Attempt to update null RHI");
            BERSERK_COND_ERROR_RET(buffer->getBufferSize() >= offset + mData.size(), "Attempt to update out of buffer range");

            buffer->update(mData.size(), offset, mData.data());
        }

        bool UniformBufferWriter::checkBounds(uint32 offset, uint32 valueSize) {
            return (offset + valueSize) <= mData.size();
        }
        
    }
}
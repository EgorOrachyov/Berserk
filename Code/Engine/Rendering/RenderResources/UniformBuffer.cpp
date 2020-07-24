/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RenderResources/UniformBuffer.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        UniformBuffer::UniformBuffer(uint32 size) {
            auto& device = RHIDevice::getSingleton();

            mData.resize(size);
            mUniformBufferRHI = device.createUniformBuffer(size, EMemoryType::Dynamic, nullptr);
        }

        bool UniformBuffer::isInitialized() const {
            return mUniformBufferRHI.isNotNull();
        }

        bool UniformBuffer::isInitializedRHI() const {
            return mUniformBufferRHI.isNotNull();
        }

        void UniformBuffer::setFloat(float t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(float)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(float));
            mIsDirty = true;
        }

        void UniformBuffer::setVec2(const UniformBuffer::Vec2 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec2)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(Vec2));
            mIsDirty = true;
        }

        void UniformBuffer::setVec3(const UniformBuffer::Vec3 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec3)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(Vec3));
            mIsDirty = true;
        }

        void UniformBuffer::setVec4(const UniformBuffer::Vec4 &t, uint32 offset) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, sizeof(Vec4)), "Out of bounds value");

            auto d = mData.data();
            Memory::copy(d + offset, &t, sizeof(Vec4));
            mIsDirty = true;
        }

        void UniformBuffer::setMat2(const UniformBuffer::Mat2 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat2::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat2::dimM(); i++) {
                Memory::copy(d + i * stride, tt.data() + i * Mat2::stride(), Mat2::stride());
            }

            mIsDirty = true;
        }

        void UniformBuffer::setMat3(const UniformBuffer::Mat3 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat3::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat3::dimM(); i++) {
                Memory::copy(d + i * stride, tt.data() + i * Mat3::stride(), Mat3::stride());
            }

            mIsDirty = true;
        }

        void UniformBuffer::setMat4(const UniformBuffer::Mat4 &t, uint32 offset, uint32 stride, bool transpose) {
            BERSERK_COND_ERROR_RET(checkBounds(offset, stride * Mat4::dimM()), "Out of bounds value");

            auto tt = transpose? t.transpose() : t;
            auto d = mData.data() + offset;

            for (uint32 i = 0; i < Mat4::dimM(); i++) {
                Memory::copy(d + i * stride, tt.data() + i * Mat4::stride(), Mat4::stride());
            }

            mIsDirty = true;
        }

        void UniformBuffer::updateDataGPU() {
            BERSERK_COND_ERROR_RET(mUniformBufferRHI.isNotNull(), "Attempt to update null RHI");

            if (mIsDirty) {
                mUniformBufferRHI->update(mData.size(), 0, mData.data());
                mIsDirty = false;
            }
        }

        bool UniformBuffer::checkBounds(uint32 offset, uint32 valueSize) {
            return (offset + valueSize) <= mData.size();
        }

    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Rendering/Resources/UniformBuffer.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Rendering {

        UniformBuffer::UniformBuffer(CString name, const ShaderUniformBlock &block) {
            mFriendlyName = std::move(name);
            mUniformBlockLayout = block;
            auto size = mUniformBlockLayout->getSize();
            mCachedUniformData.resize(size);

            auto& device = RHIDevice::getSingleton();
            mUniformBuffer = device.createUniformBuffer(size, EMemoryType::Dynamic, nullptr);

            if (mUniformBuffer.isNotNull()) linkResource();
        }

        UniformBuffer::~UniformBuffer() {
            unlinkResource();
        }

        bool UniformBuffer::isInitialized() const {
            return mUniformBuffer.isNotNull();
        }

        bool UniformBuffer::isInitializedRHI() const {
            return mUniformBuffer.isNotNull();
        }

        CString UniformBuffer::getFriendlyName() const {
            return mFriendlyName;
        }

        bool UniformBuffer::setFloat(const CString &name, float value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Float1) {
                Memory::copy(mCachedUniformData.data() + member->getOffset(), &value, sizeof(float));
                return true;
            }
            return false;
        }

        bool UniformBuffer::setVec2(const CString &name, const Vec2 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Float2) {
                Memory::copy(mCachedUniformData.data() + member->getOffset(), value.getData(), sizeof(Vec2));
                return true;
            }
            return false;
        }

        bool UniformBuffer::setVec3(const CString &name, const Vec3 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Float3) {
                Memory::copy(mCachedUniformData.data() + member->getOffset(), value.getData(), sizeof(Vec3));
                return true;
            }
            return false;
        }

        bool UniformBuffer::setVec4(const CString &name, const Vec4 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Float4) {
                Memory::copy(mCachedUniformData.data() + member->getOffset(), value.getData(), sizeof(Vec4));
                return true;
            }
            return false;
        }

        bool UniformBuffer::setMat2(const Berserk::CString &name, const Mat2 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Mat2) {
                bool rowMajor = member->getIsRowMajor();
                auto valueWrite = (rowMajor? value: value.transpose());
                uint32 matrixStride = member->getMatrixStride();
                uint32 rowStride = sizeof(float) * 2;
                uint8* dest = mCachedUniformData.data() + member->getOffset();
                uint8* src = (uint8*)valueWrite.data();

                for (uint32 i = 0; i < 2; i++) {
                    Memory::copy(dest, src, rowStride);
                    dest += matrixStride;
                    src += rowStride;
                }

                return true;
            }
            return false;
        }

        bool UniformBuffer::setMat3(const Berserk::CString &name, const Mat3 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Mat3) {
                bool rowMajor = member->getIsRowMajor();
                auto valueWrite = (rowMajor? value: value.transpose());
                uint32 matrixStride = member->getMatrixStride();
                uint32 rowStride = sizeof(float) * 3;
                uint8* dest = mCachedUniformData.data() + member->getOffset();
                uint8* src = (uint8*)valueWrite.data();

                for (uint32 i = 0; i < 3; i++) {
                    Memory::copy(dest, src, rowStride);
                    dest += matrixStride;
                    src += rowStride;
                }

                return true;
            }
            return false;
        }

        bool UniformBuffer::setMat4(const CString &name, const Mat4 &value) {
            auto member = mUniformBlockLayout->findMember(name);
            if (member.isNotNull() && member->getBaseType() == EShaderData::Mat4) {
                bool rowMajor = member->getIsRowMajor();
                auto valueWrite = (rowMajor? value: value.transpose());
                uint32 matrixStride = member->getMatrixStride();
                uint32 rowStride = sizeof(float) * 4;
                uint8* dest = mCachedUniformData.data() + member->getOffset();
                uint8* src = (uint8*)valueWrite.data();

                for (uint32 i = 0; i < 4; i++) {
                    Memory::copy(dest, src, rowStride);
                    dest += matrixStride;
                    src += rowStride;
                }

                return true;
            }
            return false;
        }

        void UniformBuffer::updateUniformBufferDataGPU() {
            if (mUniformBuffer.isNotNull()) {
                mUniformBuffer->update(mCachedUniformData.size(), 0, mCachedUniformData.data());
            }
        }

    }
}
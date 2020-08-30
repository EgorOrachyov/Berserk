/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <UniformBlockLayoutBuilder.h>

namespace Berserk {
    namespace Render {

        UniformBlockLayoutBuilder& UniformBlockLayoutBuilder::setName(CString name) {
            mName = std::move(name);
            return *this;
        }

        UniformBlockLayoutBuilder& UniformBlockLayoutBuilder::addMember(CString name, EShaderData type) {
            BERSERK_COND_ERROR_RET_VALUE(*this, !mMembersIdx.contains(name), "Member (%s) already present in uniform block: %s", name.data(), mName.data());
            BERSERK_COND_ERROR_RET_VALUE(*this, isValidUniformMemberType(type), "Invalid member type (%s) for uniform block: %s", name.data(), mName.data());

            uint32 alignedOffset = getBaseAlign(type);
            uint32 currentOffset = mSize;
            currentOffset += currentOffset % alignedOffset? alignedOffset - (currentOffset % alignedOffset): 0;

            uint32 offset = currentOffset;
            uint32 stride = 0;
            uint32 matrixStride = getMatrixStride(type);
            uint32 elements = 1;
            uint32 size = RHIDefinitionsUtil::getShaderDataSize(type);
            EShaderData baseType = type;
            bool rowMajor = false;

            uint32 memberIdx = mMembers.size();
            UniformBlockMember member(name, offset, stride, matrixStride, elements, size, baseType, rowMajor);

            mMembers.emplace(std::move(member));
            mMembersIdx.move(name, memberIdx);

            mSize = currentOffset + getStride(type);

            return *this;
        }

        UniformBlockLayoutBuilder& UniformBlockLayoutBuilder::addMemberArray(CString name, EShaderData type, uint32 arraySize) {
            BERSERK_COND_ERROR_RET_VALUE(*this, !mMembersIdx.contains(name), "Member (%s) already present in uniform block: %s", name.data(), mName.data());
            BERSERK_COND_ERROR_RET_VALUE(*this, isValidUniformMemberType(type), "Invalid member type (%s) for uniform block: %s", name.data(), mName.data());
            BERSERK_COND_ERROR_RET_VALUE(*this, arraySize > 1, "Invalid member array size (%s) for uniform block: %s", name.data(), mName.data());

            // Align first array element that its offset is multiple of arrayBase
            uint32 alignedOffset = getBaseAlignArray(type);
            uint32 currentOffset = mSize;
            currentOffset += currentOffset % alignedOffset? alignedOffset - (currentOffset % alignedOffset): 0;

            uint32 offset = currentOffset;
            uint32 stride = getStrideArray(type);
            uint32 matrixStride = getMatrixStride(type);
            uint32 elements = arraySize;
            uint32 size = RHIDefinitionsUtil::getShaderDataSize(type);
            EShaderData baseType = type;
            bool rowMajor = false;

            uint32 memberIdx = mMembers.size();
            UniformBlockMember member(name, offset, stride, matrixStride, elements, size, baseType, rowMajor);

            mMembers.emplace(std::move(member));
            mMembersIdx.move(name, memberIdx);

            mSize = currentOffset + stride * elements;

            return *this;
        }

        void UniformBlockLayoutBuilder::buildMove(UniformBlockLayout &layout) {
            layout.mName = std::move(mName);
            layout.mMembers = std::move(mMembers);
            layout.mMembersIdx = std::move(mMembersIdx);
            layout.mSize = mSize;
        }

        uint32 UniformBlockLayoutBuilder::getBaseAlign(EShaderData shaderData) {
            switch (shaderData) {
                case EShaderData::Float1:
                    return sizeof(float) * 1;
                case EShaderData::Float2:
                    return sizeof(float) * 2;
                case EShaderData::Float3:
                case EShaderData::Float4:
                    return sizeof(float) * 4;
                case EShaderData::Int1:
                    return sizeof(int32) * 1;
                case EShaderData::Int2:
                    return sizeof(int32) * 2;
                case EShaderData::Int3:
                case EShaderData::Int4:
                    return sizeof(int32) * 4;
                case EShaderData::Bool1:
                    return sizeof(bool) * 1;
                case EShaderData::Bool2:
                    return sizeof(bool) * 2;
                case EShaderData::Bool3:
                case EShaderData::Bool4:
                    return sizeof(bool) * 4;
                case EShaderData::Mat2:
                    return sizeof(float) * 2;
                case EShaderData::Mat3:
                    return sizeof(float) * 4;
                case EShaderData::Mat4:
                    return sizeof(float) * 4;
                default:
                    return 0;
            }
        }
        
        uint32 UniformBlockLayoutBuilder::getBaseAlignArray(EShaderData shaderData) {
            return sizeof(float) * 4;
        }
        
        uint32 UniformBlockLayoutBuilder::getStride(EShaderData shaderData) {
            switch (shaderData) {
                case EShaderData::Float1:
                    return sizeof(float) * 1;
                case EShaderData::Float2:
                    return sizeof(float) * 2;
                case EShaderData::Float3:
                    return sizeof(float) * 3;
                case EShaderData::Float4:
                    return sizeof(float) * 4;
                case EShaderData::Int1:
                    return sizeof(int32) * 1;
                case EShaderData::Int2:
                    return sizeof(int32) * 2;
                case EShaderData::Int3:
                    return sizeof(int32) * 3;
                case EShaderData::Int4:
                    return sizeof(int32) * 4;
                case EShaderData::Bool1:
                    return sizeof(bool) * 1;
                case EShaderData::Bool2:
                    return sizeof(bool) * 2;
                case EShaderData::Bool3:
                    return sizeof(bool) * 3;
                case EShaderData::Bool4:
                    return sizeof(bool) * 4;
                case EShaderData::Mat2:
                    return sizeof(float) * 4 * 2;
                case EShaderData::Mat3:
                    return sizeof(float) * 4 * 3;
                case EShaderData::Mat4:
                    return sizeof(float) * 4 * 4;
                default:
                    return 0;
            }
        }
        
        uint32 UniformBlockLayoutBuilder::getStrideArray(EShaderData shaderData) {
            switch (shaderData) {
                case EShaderData::Float1:
                case EShaderData::Float2:
                case EShaderData::Float3:
                case EShaderData::Float4:
                    return sizeof(float) * 4;
                case EShaderData::Int1:
                case EShaderData::Int2:
                case EShaderData::Int3:
                case EShaderData::Int4:
                    return sizeof(int32) * 4;
                case EShaderData::Bool1:
                case EShaderData::Bool2:
                case EShaderData::Bool3:
                case EShaderData::Bool4:
                    return sizeof(bool) * 4;
                case EShaderData::Mat2:
                    return sizeof(float) * 4 * 2;
                case EShaderData::Mat3:
                    return sizeof(float) * 4 * 3;
                case EShaderData::Mat4:
                    return sizeof(float) * 4 * 4;
                default:
                    return 0;
            }
        }

        uint32 UniformBlockLayoutBuilder::getMatrixStride(EShaderData shaderData) {
            switch (shaderData) {
                case EShaderData::Mat2:
                    return sizeof(float) * 4;
                case EShaderData::Mat3:
                    return sizeof(float) * 4;
                case EShaderData::Mat4:
                    return sizeof(float) * 4;
                default:
                    return 0;
            }
        }

        bool UniformBlockLayoutBuilder::isValidUniformMemberType(EShaderData shaderData) {
            switch (shaderData) {
                case EShaderData::Float1:
                case EShaderData::Float2:
                case EShaderData::Float3:
                case EShaderData::Float4:
                case EShaderData::Int1:
                case EShaderData::Int2:
                case EShaderData::Int3:
                case EShaderData::Int4:
                case EShaderData::Bool1:
                case EShaderData::Bool2:
                case EShaderData::Bool3:
                case EShaderData::Bool4:
                case EShaderData::Mat2:
                case EShaderData::Mat3:
                case EShaderData::Mat4:
                    return true;
                default:
                    return false;
            }
        }

    }
}
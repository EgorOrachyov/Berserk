/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERBLOCKMEMBER_H
#define BERSERK_RHISHADERBLOCKMEMBER_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <RHI/RHIDefinitions.h>

namespace Berserk {

    /** Member description of the uniform buffer */
    class ShaderBlockMember {
    public:

        ShaderBlockMember() = default;
        ShaderBlockMember(const ShaderBlockMember& other) = default;
        ShaderBlockMember(CString name, uint32 offset, uint32 stride, uint32 matrixStride, uint32 elements, uint32 size, EShaderData dataType, bool rowMajor) {
            mName = std::move(name);
            mOffset = offset;
            mStride = stride;
            mMatrixStride = matrixStride;
            mElements = elements;
            mSize = size;
            mBaseType = dataType;
            mRowMajor = rowMajor;
        }

        /** @return Name of the member or array in the uniform block */
        const CString& getName() const { return mName; }

        /** @return Offset in the uniform buffer */
        uint32 getOffset() const { return mOffset; }

        /** @return Offset between array members */
        uint32 getStride() const { return mStride; }

        /** @return Offset between matrix rows or columns (depends on order) */
        uint32 getMatrixStride() const { return mMatrixStride; }

        /** @return Number of the elements in the array */
        uint32 getElements() const { return mElements; }

        /** @return Size of the element (base element for arrays) */
        uint32 getSize() const { return mSize; }

        /** @return Full member size in the buffer (full size for arrays) */
        uint32 getSizeFull() const { return mStride * mElements; }

        /** @return Type of the element (base element for arrays) */
        EShaderData getBaseType() const { return mBaseType; }

        /** @return Member order (for matrices) */
        bool getIsRowMajor() const { return mRowMajor; }

        /** @return True if this is array member of the block */
        bool isArray() const { return mStride > 0; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderBlockMember& member) {
            archive << member.mName;
            archive << member.mOffset;
            archive << member.mStride;
            archive << member.mMatrixStride;
            archive << member.mElements;
            archive << member.mSize;
            archive << (uint32)member.mBaseType;
            archive << member.mRowMajor;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderBlockMember& member) {
            archive >> member.mName;
            archive >> member.mOffset;
            archive >> member.mStride;
            archive >> member.mMatrixStride;
            archive >> member.mElements;
            archive >> member.mSize;
            archive >> (uint32&)member.mBaseType;
            archive >> member.mRowMajor;
            return archive;
        }

    private:
        CString mName;
        uint32 mOffset;
        uint32 mStride;
        uint32 mMatrixStride;
        uint32 mElements;
        uint32 mSize;
        EShaderData mBaseType;
        bool mRowMajor;
    };

}

#endif //BERSERK_RHISHADERBLOCKMEMBER_H
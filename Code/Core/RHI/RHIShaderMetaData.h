/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERMETADATA_H
#define BERSERK_RHISHADERMETADATA_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <RHI/RHIResources.h>

namespace Berserk {

    /** Input/input shader attribute */
    class ShaderAttribute {
    public:

        ShaderAttribute() = default;
        ShaderAttribute(CString name, uint32 location, EShaderData dataType) {
            mName = std::move(name);
            mLocation = location;
            mData = dataType;
        }

        /** @return Name of the attribute in the shader */
        const CString& getName() const { return mName; }

        /** @return Native location for shader input/output */
        uint32 getLocation() const { return mLocation; }

        /** @return Type of the param */
        EShaderData getDataType() const { return mData; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderAttribute& attribute) {
            archive << attribute.mName;
            archive << attribute.mLocation;
            archive << (uint32)attribute.mData;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderAttribute& attribute) {
            archive >> attribute.mName;
            archive >> attribute.mLocation;
            archive >> (uint32&)attribute.mData;
            return archive;
        }

    private:
        CString mName;
        uint32 mLocation;
        EShaderData mData;
    };

    /** Bindable params (textures, samplers) */
    class ShaderParam {
    public:

        ShaderParam() = default;
        ShaderParam(CString name, uint32 location, EShaderData dataType, TEnumMask<EShaderType> stageFlags) {
            mName = std::move(name);
            mLocation = location;
            mData = dataType;
            mStageFlags = stageFlags;
        }

        /** @return Name of the param in the shader */
        const CString& getName() const { return mName; }

        /** @return Native location (binding) for shader access */
        uint32 getLocation() const { return mLocation; }

        /** @return Type of the param */
        EShaderData getDataType() const { return mData; }

        /** @return Stage usage flags of the param */
        TEnumMask<EShaderType> getStageFlags() const { return mStageFlags; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderParam& param) {
            archive << param.mName;
            archive << param.mLocation;
            archive << (uint32)param.mData;
            archive << param.mStageFlags;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderParam& param) {
            archive >> param.mName;
            archive >> param.mLocation;
            archive >> (uint32&)param.mData;
            archive >> param.mStageFlags;
            return archive;
        }

    private:
        CString mName;
        uint32 mLocation;
        EShaderData mData;
        TEnumMask<EShaderType> mStageFlags;
    };

    /** Member description of the uniform buffer */
    class ShaderMember {
    public:

        ShaderMember() = default;
        ShaderMember(const ShaderMember& other) = default;
        ShaderMember(CString name, uint32 offset, uint32 stride, uint32 matrixStride, uint32 elements, uint32 size, EShaderData dataType, bool rowMajor) {
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

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderMember& member) {
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
        friend Archive& operator>>(Archive& archive, ShaderMember& member) {
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

    class ShaderUniformBlock {
    public:

        ShaderUniformBlock() = default;
        ShaderUniformBlock(const ShaderUniformBlock& other) = default;
        ShaderUniformBlock(CString name, TArray<ShaderMember> &members, uint32 binding, uint32 size, TEnumMask<EShaderType> flags);

        /** @return Shader uniform block member found list raw search (possibly null) */
        TRef<const ShaderMember> getMember(const char* name) const;

        /** @return Shader uniform block member found via map look-up (possibly null) */
        TRef<const ShaderMember> findMember(const CString& name) const;

        /** @return Uniform binding name */
        const CString &getName() const { return mName; }

        /** @return Members of the buffer */
        const TArray<ShaderMember> &getMembers() const { return mMembers; }

        /** @return Native buffer binding */
        uint32 getBinding() const { return mBinding; }

        /** @return Size of the buffer  */
        uint32 getSize() const { return mSize; }

        /** @return Stage usage flags of the buffer */
        TEnumMask<EShaderType> getStageFlags() const { return mStageFlags; }

        /** Serialization for shader caching */
        friend Archive& operator<<(Archive& archive, const ShaderUniformBlock& block) {
            archive << block.mName;
            archive << block.mMembersIdx;
            archive << block.mMembers;
            archive << block.mBinding;
            archive << block.mSize;
            archive << block.mStageFlags;
            return archive;
        }

        /** Deserialization for shader caching */
        friend Archive& operator>>(Archive& archive, ShaderUniformBlock& block) {
            archive >> block.mName;
            archive >> block.mMembersIdx;
            archive >> block.mMembers;
            archive >> block.mBinding;
            archive >> block.mSize;
            archive >> block.mStageFlags;
            return archive;
        }

    private:
        CString mName;
        TMap<CString,uint32> mMembersIdx;
        TArray<ShaderMember> mMembers;
        uint32 mBinding;
        uint32 mSize;
        TEnumMask<EShaderType> mStageFlags;
    };

    class RHIShaderMetaData : public RHIResource {
    public:
        ~RHIShaderMetaData() override = default;

        static const uint32 MAX_UNIFORM_NAME_LENGTH = 512;
        static const uint32 MAX_UNIFORMS_COUNT = 512;

        /** @return Shader param found via raw search (possibly null) */
        TRef<const ShaderParam> getParam(const char* name) const;

        /** @return Shader uniform block found via raw search (possibly null) */
        TRef<const ShaderUniformBlock> getUniformBlock(const char* name) const;

        /** @return Shader param found via map look-up (possibly null) */
        TRef<const ShaderParam> findParam(const CString& name) const;

        /** @return Shader uniform block found via map look-up (possibly null) */
        TRef<const ShaderUniformBlock> findUniformBlock(const CString& name) const;

        /** @return Program variables */
        const TArray<ShaderParam> &getParams() const { return mParams; }

        /** @return Program uniform blocks info */
        const TArray<ShaderUniformBlock> &getUniformBlocks() const { return mUniformBlocks; }

        /** @return Input attributes of the vertex shader (as main entry for graphics shader)*/
        const TArray<ShaderAttribute> &getVertexShaderAttributes() const { return mVertexShaderAttributes; }

        /** @return Input attributes of the fragment shader */
        const TArray<ShaderAttribute> &getFragmentShaderAttributes() const { return mFragmentShaderAttributes; }

        /** @return Descriptive primitive type name (for debug) */
        static const char *getShaderDataName(EShaderData dataType);

        /** Serialization for shader caching */
        virtual void serialize(Archive& archive) const = 0;

        /** Deserialization for shader caching */
        virtual void deserialize(Archive& archive) = 0;

        /** Serialization for shader caching (syntax sugar) */
        friend Archive& operator<<(Archive& archive, const RHIShaderMetaData& meta) {
            meta.serialize(archive);
            return archive;
        }

        /** Deserialization for shader caching (syntax sugar) */
        friend Archive& operator>>(Archive& archive, RHIShaderMetaData& meta) {
            meta.deserialize(archive);
            return archive;
        }

    protected:
        TMap<CString,uint32> mParamsIdx;
        TMap<CString,uint32> mUniformBlocksIdx;
        TArray<ShaderParam> mParams;
        TArray<ShaderUniformBlock> mUniformBlocks;
        TArray<ShaderAttribute> mVertexShaderAttributes;
        TArray<ShaderAttribute> mFragmentShaderAttributes;
    };

}

#endif //BERSERK_RHISHADERMETADATA_H
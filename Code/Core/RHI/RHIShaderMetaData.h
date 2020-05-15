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

    private:
        CString mName;
        uint32 mLocation;
        EShaderData mData;
    };

    /** Bindable params (textures, samplers) */
    class ShaderParam {
    public:

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

    private:
        CString mName;
        uint32 mLocation;
        EShaderData mData;
        TEnumMask<EShaderType> mStageFlags;
    };

    /** Member description of the uniform buffer */
    class ShaderMember {
    public:

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

        ShaderUniformBlock(CString name, TArray<ShaderMember> &members, uint32 binding, uint32 size, TEnumMask<EShaderType> flags) {
            mName = std::move(name);
            mMembers = std::move(members);
            mBinding = binding;
            mSize = size;
            mStageFlags = flags;

            uint32 idx = 0;
            for (auto& m: mMembers) {
                mMembersIdx.add(m.getName(), idx);
                idx += 1;
            }
        }

        TRef<const ShaderMember> getMember(const char* name) const {
            for (const auto& m: mMembers) {
                if (m.getName() == name) {
                    return m;
                }
            }

            return {};
        }

        /** @return Shader uniform block member found via map look-up (possibly null) */
        TRef<const ShaderMember> findMember(const CString& name) const {
            auto found = mMembersIdx.getPtr(name);
            if (found.isNotNull()) {
                return mMembers[*found];
            }
            return {};
        }

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

        TRef<const ShaderParam> getParam(const char* name) const {
            for (const auto& p: mParams) {
                if (p.getName() == name) {
                    return p;
                }
            }

            return {};
        }

        TRef<const ShaderUniformBlock> getUniformBlock(const char* name) const {
            for (const auto& b: mUniformBlocks) {
                if (b.getName() == name) {
                    return b;
                }
            }

            return {};
        }

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
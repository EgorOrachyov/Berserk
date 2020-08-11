/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHISHADERUNIFORMBLOCK_H
#define BERSERK_RHISHADERUNIFORMBLOCK_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <TEnumMask.h>
#include <RHI/RHIShaderBlockMember.h>

namespace Berserk {

    class ShaderUniformBlock {
    public:

        ShaderUniformBlock() = default;
        ShaderUniformBlock(const ShaderUniformBlock& other) = default;
        ShaderUniformBlock(CString name, TArray<ShaderBlockMember> &members, uint32 binding, uint32 size, TEnumMask<EShaderType> flags);

        /** @return Shader uniform block member found list raw search (possibly null) */
        TRef<const ShaderBlockMember> getMember(const char* name) const;

        /** @return Shader uniform block member found via map look-up (possibly null) */
        TRef<const ShaderBlockMember> findMember(const CString& name) const;

        /** @return Uniform binding name */
        const CString &getName() const { return mName; }

        /** @return Members of the buffer */
        const TArray<ShaderBlockMember> &getMembers() const { return mMembers; }

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
        TArray<ShaderBlockMember> mMembers;
        uint32 mBinding;
        uint32 mSize;
        TEnumMask<EShaderType> mStageFlags;
    };

}

#endif //BERSERK_RHISHADERUNIFORMBLOCK_H
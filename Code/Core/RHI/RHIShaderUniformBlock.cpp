/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIShaderUniformBlock.h>

namespace Berserk {

    ShaderUniformBlock::ShaderUniformBlock(CString name, TArray<ShaderBlockMember> &members,
                                           uint32 binding, uint32 size,
                                           TEnumMask<EShaderType> flags) {
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

    TRef<const ShaderBlockMember> ShaderUniformBlock::getMember(const char *name) const {
        for (const auto& m: mMembers) {
            if (m.getName() == name) {
                return m;
            }
        }

        return {};
    }

    TRef<const ShaderBlockMember> ShaderUniformBlock::findMember(const CString &name) const {
        auto found = mMembersIdx.getPtr(name);
        if (found.isNotNull()) {
            return mMembers[*found];
        }
        return {};
    }

}
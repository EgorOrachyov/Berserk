/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <RHI/RHIShaderMetaData.h>
#include "RHIDefinitions.h"

namespace Berserk {

    ShaderUniformBlock::ShaderUniformBlock(CString name, TArray<ShaderMember> &members,
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
    
    TRef<const ShaderMember> ShaderUniformBlock::getMember(const char *name) const {
        for (const auto& m: mMembers) {
            if (m.getName() == name) {
                return m;
            }
        }

        return {};
    }

    TRef<const ShaderMember> ShaderUniformBlock::findMember(const CString &name) const {
        auto found = mMembersIdx.getPtr(name);
        if (found.isNotNull()) {
            return mMembers[*found];
        }
        return {};
    }

    TRef<const ShaderParam> RHIShaderMetaData::getParam(const char *name) const {
        for (const auto& p: mParams) {
            if (p.getName() == name) {
                return p;
            }
        }

        return {};
    }

    TRef<const ShaderUniformBlock> RHIShaderMetaData::getUniformBlock(const char *name) const {
        for (const auto& b: mUniformBlocks) {
            if (b.getName() == name) {
                return b;
            }
        }

        return {};
    }

    TRef<const ShaderParam> RHIShaderMetaData::findParam(const CString &name) const {
        auto found = mParamsIdx.getPtr(name);
        if (found.isNotNull()) {
            return mParams[*found];
        }
        return {};
    }

    TRef<const ShaderUniformBlock> RHIShaderMetaData::findUniformBlock(const CString &name) const {
        auto found = mUniformBlocksIdx.getPtr(name);
        if (found.isNotNull()) {
            return mUniformBlocks[*found];
        }
        return {};
    }
    
    const char * RHIShaderMetaData::getShaderDataName(EShaderData dataType) {
        switch (dataType) {
            case EShaderData::Float1:
                return "Float1";
            case EShaderData::Float2:
                return "Float2";
            case EShaderData::Float3:
                return "Float3";
            case EShaderData::Float4:
                return "Float4";
            case EShaderData::Mat2:
                return "Mat2";
            case EShaderData::Mat3:
                return "Mat3";
            case EShaderData::Mat4:
                return "Mat4";
            case EShaderData::Sampler2D:
                return "Sampler2D";
            case EShaderData::Sampler3D:
                return "Sampler3D";
            case EShaderData::SamplerCube:
                return "SamplerCube";
            default:
                return "Undefined";
        }
    }

}
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

    void RHIShaderMetaData::showDebugInfo() const {
        printf("Shader meta info:\n");
        printf(" Vertex Inputs:\n");
        for (auto& i: mVertexShaderAttributes) {
            printf("  Input: Name=%s,Location=%i,Type=%s\n",
                   i.getName().data(), i.getLocation(),
                   RHIDefinitionsUtil::getVertexElementStringFromEnum(i.getDataType()));
        }

        printf(" Params:\n");
        for (auto& p: mParams) {
            printf("  Param: Name=%s,Location=%u\n",
                    p.getName().data(), p.getLocation());
        }

        printf(" UniformBlocks:\n");
        for (auto& b: mUniformBlocks) {
            printf("  Block: Name=%s,binding=%u,size=%u\n",
                    b.getName().data(), b.getBinding(), b.getSize());

            for (auto& m: b.getMembers()) {
                printf("   Member: Name=%s,Offset=%u,Size=%u,Elements=%u,Stride=%u,MatrixStride=%u,RowMajor=%u,BaseType=%s\n",
                       m.getName().data(), m.getOffset(), m.getSize(), m.getElements(),
                       m.getStride(), m.getMatrixStride(), m.getIsRowMajor(),
                       RHIDefinitionsUtil::getShaderDataStringFromEnum(m.getBaseType()));
            }
        }
    }

}
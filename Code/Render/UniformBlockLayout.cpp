/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <UniformBlockLayout.h>
#include <UniformBlockLayoutBuilder.h>

namespace Berserk {
    namespace Render {

        bool UniformBlockLayout::hasMember(const CString &name) const {
            return mMembersIdx.contains(name);
        }

        TRef<const UniformBlockMember> UniformBlockLayout::getMember(const CString &name) const {
            auto ptrIdx = mMembersIdx.getPtr(name);
            return ptrIdx.isNotNull() ? TRef<const UniformBlockMember>(mMembers[*ptrIdx]): nullptr;
        }

        void UniformBlockLayout::showDebugInfo() {
            printf("Block: Name=%s,size=%u\n",
                   getName().data(), getSize());

            for (auto& m: getMembers()) {
                printf(" Member: Name=%s,Offset=%u,Size=%u,Elements=%u,Stride=%u,MatrixStride=%u,RowMajor=%u,BaseType=%s\n",
                       m.getName().data(), m.getOffset(), m.getSize(), m.getElements(),
                       m.getStride(), m.getMatrixStride(), m.getIsRowMajor(),
                       RHIDefinitionsUtil::getShaderDataStringFromEnum(m.getBaseType()));
            }
        }

    }
}
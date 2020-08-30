/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIFORMBLOCKLAYOUT_H
#define BERSERK_UNIFORMBLOCKLAYOUT_H

#include <Containers/TMap.h>
#include <Containers/TArray.h>
#include <RHI/RHIShaderBlockMember.h>

namespace Berserk {
    namespace Render {

        using UniformBlockMember = ShaderBlockMember;

        /** CPU side uniform block layout for safe uniform transfer to GPU */
        class UniformBlockLayout {
        public:
            UniformBlockLayout() = default;
            ~UniformBlockLayout() = default;

            /** @return True if has member with specified name */
            bool hasMember(const CString& name) const;

            /** @return Member with specified name */
            TRef<const UniformBlockMember> getMember(const CString& name) const;

            /** @return Block name */
            const CString& getName() const { return mName; }

            /** @return Ordered members list of this block */
            const TArray<UniformBlockMember> &getMembers() const { return mMembers; }

            /** @return Block size in bytes */
            uint32 getSize() const { return mSize; }

            /** Prints debug info of this layout */
            void showDebugInfo();

        private:
            friend class UniformBlockLayoutBuilder;

            CString mName;
            TMap<CString,uint32> mMembersIdx;
            TArray<UniformBlockMember> mMembers;
            uint32 mSize = 0;
        };

    }
}




#endif //BERSERK_UNIFORMBLOCKLAYOUT_H

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIFORMBLOCKLAYOUTBUILDER_H
#define BERSERK_UNIFORMBLOCKLAYOUTBUILDER_H

#include <UniformBlockLayout.h>
#include <TPtrShared.h>

namespace Berserk {
    namespace Render {

        /** Helper for block layout builder for safe uniforms setup */
        class UniformBlockLayoutBuilder {
        public:

            /** Specify block name */
            UniformBlockLayoutBuilder& setName(CString name);

            /** Adds member of type if not already present in the block */
            UniformBlockLayoutBuilder& addMember(CString name, EShaderData type);

            /** Adds array member of type if not already present in the block (array size must be more than 1) */
            UniformBlockLayoutBuilder& addMemberArray(CString name, EShaderData type, uint32 arraySize);

            /** Build provided layout (note: builder will be invalidated after this operation for more friendly usage) */
            void buildMove(UniformBlockLayout &layout);

        private:
            static uint32 getBaseAlign(EShaderData shaderData);
            static uint32 getBaseAlignArray(EShaderData shaderData);
            static uint32 getStride(EShaderData shaderData);
            static uint32 getStrideArray(EShaderData shaderData);
            static uint32 getMatrixStride(EShaderData shaderData);
            static bool isValidUniformMemberType(EShaderData shaderData);

            CString mName;
            TMap<CString,uint32> mMembersIdx;
            TArray<UniformBlockMember> mMembers;
            uint32 mSize = 0;
            uint32 mPrevStride = 0;
        };

    }
}




#endif //BERSERK_UNIFORMBLOCKLAYOUTBUILDER_H

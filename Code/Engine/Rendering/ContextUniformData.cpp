/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ContextUniformData.h>

namespace Berserk {
    namespace Render {
        
        void ContextUniformData::addBufferForBlock(const CString &blockName, const TPtrShared<UniformBuffer> &buffer) {
            BERSERK_COND_ERROR_RET(buffer.isNotNull(), "Buffer must be valid");
            BERSERK_COND_ERROR_RET(!mBuffers.contains(blockName), "Entry with the same name [%s] already present", blockName.data());

            mBuffersNames.add(blockName);
            mBuffers.add(blockName, buffer);
        }
        
        TPtrShared<UniformBuffer> ContextUniformData::findBufferForBlock(const CString &blockName) {
            auto entry = mBuffers.getPtr(blockName);
            return entry.isNotNull() ? *entry: nullptr;
        }
        
    }
}
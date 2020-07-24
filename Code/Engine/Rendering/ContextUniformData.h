/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONTEXTUNIFORMDATA_H
#define BERSERK_CONTEXTUNIFORMDATA_H

#include <Containers/TMap.h>
#include <RenderResources/UniformBuffer.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Uniform data context
         *
         * Context of global uniform data, which could be shared among
         * several shaders. Such data is camera setting, scene settings and
         * post processing configuration.
         */
        class ContextUniformData {
        public:

            /** Add buffer in the context for specified block */
            void addBufferForBlock(const CString &blockName, const TPtrShared<UniformBuffer> &buffer);

            /** @return Uniform buffer for specified uniform block name */
            TPtrShared<UniformBuffer> findBufferForBlock(const CString &blockName);

            /** @return List of uniform buffers in this env context */
            const TArray<CString> &getBuffersNames() { return mBuffersNames; }

        private:
            TArray<CString> mBuffersNames;
            TMap<CString,TPtrShared<UniformBuffer>> mBuffers;
        };

    }
}

#endif //BERSERK_CONTEXTUNIFORMDATA_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STATICINDEXBUFFERUINT32_H
#define BERSERK_STATICINDEXBUFFERUINT32_H

#include <Rendering/Resources/IndexBuffer.h>

namespace Berserk {
    namespace Rendering {

        /** Static index buffer with uint32 indices type */
        class StaticIndexBufferUint32 final : public IndexBuffer {
        public:
            StaticIndexBufferUint32(uint32 count, const uint32 *data);
            StaticIndexBufferUint32(const TArray<uint32> &indices);
            ~StaticIndexBufferUint32() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;
        };

    }
}




#endif //BERSERK_STATICINDEXBUFFERUINT32_H

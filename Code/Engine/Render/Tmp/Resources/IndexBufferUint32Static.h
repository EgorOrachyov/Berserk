/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_INDEXBUFFERUINT32STATIC_H
#define BERSERK_INDEXBUFFERUINT32STATIC_H

#include <Tmp/Resources/IndexBuffer.h>

namespace Berserk {
    namespace Rendering {

        /** Static index buffer with uint32 indices type */
        class IndexBufferUint32Static final : public IndexBuffer {
        public:
            IndexBufferUint32Static(uint32 count, const uint32 *data);
            IndexBufferUint32Static(const TArray<uint32> &indices);
            ~IndexBufferUint32Static() override;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;
            CString getFriendlyName() const override;
        };

    }
}




#endif //BERSERK_INDEXBUFFERUINT32STATIC_H

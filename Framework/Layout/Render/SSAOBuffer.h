//
// Created by Egor Orachyov on 21.08.2018.
//

#ifndef BERSERKENGINE_SSAOBUFFER_H
#define BERSERKENGINE_SSAOBUFFER_H

#include "Essential/Types.h"

namespace Berserk
{
    enum SSAOInfo
    {
        SSAO_MAX_KERNEL_SIZE    = 64,
        SSAO_MAX_NOISE_SIZE     = 8,

        SSAO_KERNEL_SIZE    = 64,
        SSAO_NOISE_SIZE     = 4,
    };

    class SSAOBuffer
    {
    public:

        virtual ~SSAOBuffer() = default;

        virtual void create(UINT32 width, UINT32 height) = 0;
        virtual void destroy() = 0;
        virtual void useAsFBO() = 0;
        virtual void useAsUniform(UINT32 textureSlot) = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_SSAOBUFFER_H

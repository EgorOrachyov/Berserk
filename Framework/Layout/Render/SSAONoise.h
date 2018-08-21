//
// Created by Egor Orachyov on 21.08.2018.
//

#ifndef BERSERKENGINE_SSAONOISE_H
#define BERSERKENGINE_SSAONOISE_H

#include "Essential/Types.h"

namespace Berserk
{

    class SSAONoise
    {
    public:

        virtual ~SSAONoise() = default;

        virtual void create(UINT32 size) = 0;
        virtual void destroy() = 0;
        virtual void useAsUniform(UINT32 textureSlot) const = 0;

        virtual UINT32 getSize() const = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_SSAONOISE_H

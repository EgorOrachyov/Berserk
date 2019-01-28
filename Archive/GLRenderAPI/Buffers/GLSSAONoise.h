//
// Created by Egor Orachyov on 21.08.2018.
//

#ifndef BERSERKENGINE_GLSSAONOISE_H
#define BERSERKENGINE_GLSSAONOISE_H

#include "Render/SSAONoise.h"

namespace Berserk
{

    class GLSSAONoise : public SSAONoise
    {
    public:

        virtual ~GLSSAONoise() = default;

        void create(UINT32 size) override;
        void destroy() override;
        void useAsUniform(UINT32 binding) const override;

        UINT32 getSize() const override;

    private:

        UINT32 mHandle;
        UINT32 mSize;
    };

} //namespace Berserk

#endif //BERSERKENGINE_GLSSAONOISE_H
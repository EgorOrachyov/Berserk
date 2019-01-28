//
// Created by Egor Orachyov on 21.08.2018.
//

#ifndef BERSERKENGINE_GLSSAOBUFFER_H
#define BERSERKENGINE_GLSSAOBUFFER_H

#include "Render/SSAOBuffer.h"

namespace Berserk
{

    class GLSSAOBuffer : public SSAOBuffer
    {
    public:

        virtual ~GLSSAOBuffer() = default;

        void create(UINT32 width, UINT32 height) override;
        void destroy() override;
        void useAsFBO() override;
        void useAsUniform(UINT32 binding) override;

    private:

        UINT32 mFBOHandle;
        UINT32 mTextureHandle;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLSSAOBUFFER_H
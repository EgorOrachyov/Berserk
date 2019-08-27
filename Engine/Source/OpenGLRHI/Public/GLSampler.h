//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLSAMPLER_H
#define BERSERK_GLSAMPLER_H

#include <RHI/RHISampler.h>
#include <GLCommon.h>
#include <Math/Vec4f.h>

namespace Berserk
{

    class GLSampler final : public RHISampler
    {
    public:

        GLSampler(ESamplerFilter min, ESamplerFilter mag, ESamplerWrapMode wrapMode,
                  const Vec4f &borderColor, GLint min_gl, GLint mag_gl, GLint wrapMode_gl);

        ~GLSampler() override;

        void bind(uint32 textureSlot) const override;

        ESamplerFilter getMinFilter() const override;

        ESamplerFilter getMagFilter() const override;

        ESamplerWrapMode getWrapMode() const override;

        const Vec4f &getBorderColor() const override;

    private:

        GLuint mResourceID = 0;
        ESamplerFilter mMinFilter;
        ESamplerFilter mMagFilter;
        ESamplerWrapMode mWrapMode;
        Vec4f mBorderColor;

    };

} // namespace Berserk

#endif //BERSERK_GLSAMPLER_H

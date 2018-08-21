//
// Created by Egor Orachyov on 21.08.2018.
//

#include "Render/SSAOBuffer.h"
#include "Buffers/GLSSAONoise.h"
#include "Essential/GLInclude.h"
#include "Math/Vector3f.h"
#include "Math/UtilityNumbers.h"

namespace Berserk
{

    void GLSSAONoise::create(UINT32 size)
    {
        const UINT32 MAX_NOISE = SSAOInfo::SSAO_MAX_NOISE_SIZE;
        mSize = min(MAX_NOISE, size);

        Vector3f noise[MAX_NOISE * MAX_NOISE];
        for (UINT32 i = 0; i < size * size; ++i)
        {
            noise[i] = Vector3f(random(-1.0f,1.0f), random(-1.0f,1.0f), 0.0);

            printf("%f %f %f \n", noise[i].x, noise[i].y, noise[i].z);
        }

        glGenTextures(1, &mHandle);
        glBindTexture(GL_TEXTURE_2D, mHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, noise);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLSSAONoise::destroy()
    {
        if (mHandle)
        {
            glDeleteTextures(1, &mHandle);
            mHandle = 0;
        }

        mSize = 0;
    }

    void GLSSAONoise::useAsUniform(UINT32 textureSlot) const
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, mHandle);
    }

    UINT32 GLSSAONoise::getSize() const
    {
        return mSize;
    }

} //namespace Berserk
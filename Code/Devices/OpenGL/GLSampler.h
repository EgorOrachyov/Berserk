/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLSAMPLER_H
#define BERSERK_GLSAMPLER_H

#include <RHI/RHIResources.h>
#include <RHI/RHIDescs.h>
#include <GLDefinitions.h>
#include <GLErrors.h>

namespace Berserk {

    class GLSampler : public RHISampler {
    public:

        ~GLSampler() override {
            destroy();
        }

        bool create(const RHISamplerDesc& samplerDesc) {
            GLenum min;

            if (samplerDesc.useMips) {
                min = GLDefinitions::getSamplerFiltering(samplerDesc.min, samplerDesc.mipmapMode);
            }
            else {
                min = GLDefinitions::getSamplerFiltering(samplerDesc.min);
            }

            auto mag = GLDefinitions::getSamplerFiltering(samplerDesc.mag);
            auto repeatS = GLDefinitions::getSamplerRepeatMode(samplerDesc.u);
            auto repeatT = GLDefinitions::getSamplerRepeatMode(samplerDesc.v);
            auto repeatR = GLDefinitions::getSamplerRepeatMode(samplerDesc.w);
            auto color = GLDefinitions::getBorderColor(samplerDesc.color);

            glGenSamplers(1, &mSamplerHandle);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_MIN_FILTER, min);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_MAG_FILTER, mag);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_WRAP_S, repeatS);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_WRAP_T, repeatT);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_WRAP_R, repeatR);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_MIN_LOD, samplerDesc.minLod);
            glSamplerParameteri(mSamplerHandle, GL_TEXTURE_MAX_LOD, samplerDesc.maxLod);
            glSamplerParameterfv(mSamplerHandle, GL_TEXTURE_BORDER_COLOR, color.values);

            BERSERK_CATCH_OPENGL_ERRORS();

            mMin = samplerDesc.min;
            mMag = samplerDesc.mag;
            mMipmapMode = samplerDesc.mipmapMode;
            mRepeatU = samplerDesc.u;
            mRepeatV = samplerDesc.v;
            mRepeatW = samplerDesc.w;
            mBorderColor = samplerDesc.color;
            mUseMips = samplerDesc.useMips;
            mMinLod = samplerDesc.minLod;
            mMaxLod = samplerDesc.maxLod;

            return true;
        }

        void destroy() {
            if (mSamplerHandle) {
                glDeleteSamplers(1, &mSamplerHandle);
                mSamplerHandle = 0;

                BERSERK_CATCH_OPENGL_ERRORS();
            }
        }

        bool compatible(const RHISamplerDesc& samplerDesc) const {
            bool compatible = true;

            compatible = compatible && (mMin == samplerDesc.min);
            compatible = compatible && (mMag == samplerDesc.mag);
            compatible = compatible && (mMipmapMode == samplerDesc.mipmapMode);
            compatible = compatible && (mRepeatU == samplerDesc.u);
            compatible = compatible && (mRepeatV == samplerDesc.v);
            compatible = compatible && (mRepeatW == samplerDesc.w);
            compatible = compatible && (mBorderColor == samplerDesc.color);
            compatible = compatible && (mMinLod == samplerDesc.minLod);
            compatible = compatible && (mMaxLod == samplerDesc.maxLod);
            compatible = compatible && (mUseMips == samplerDesc.useMips);

            return compatible;
        }

        void bind(uint32 unit) {
            glBindSampler(unit, mSamplerHandle);
        }

        GLuint getSamplerHandle() const {
            return mSamplerHandle;
        }

        static TPtrShared<GLSampler> createSampler(const RHISamplerDesc& samplerDesc) {
            for (const auto& sampler: mCachedSamplers) {
                if (sampler->compatible(samplerDesc)) {
                    return sampler;
                }
            }

            auto sampler = TPtrShared<GLSampler>::make();
            auto result = sampler->create(samplerDesc);

            if (result) {
                mCachedSamplers.add(sampler);
                return sampler;
            }

            return nullptr;
        }

        static void clearCachedSamplers() {
            mCachedSamplers.releaseInternalMemory();
        }

    private:

        GLuint mSamplerHandle = 0;

        /** Cache created samplers in with settings */
        static TArray<TPtrShared<GLSampler>> mCachedSamplers;

    };

}

#endif //BERSERK_GLSAMPLER_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLSAMPLER_HPP
#define BERSERK_GLSAMPLER_HPP

#include <BerserkRHI/RHISampler.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLSampler: public Sampler {
        public:
            explicit GLSampler(const Desc& desc);
            ~GLSampler() override;

            void Initialize();

            GLuint GetHandle() const {
                return mHandle;
            }

        protected:
            void OnReleased() const override;

            GLuint mHandle = 0;
        };

    }
}

#endif //BERSERK_GLSAMPLER_HPP
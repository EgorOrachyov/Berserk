/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RANDOM_H
#define BERSERK_RANDOM_H

#include <BerserkCore/Platform/Platform.hpp>
#include <random>

namespace Berserk {

    class Random final {
    public:

        explicit Random() {
            mEngine.seed(0);
        };

        explicit Random(uint32 seed) {
            mEngine.seed(seed);
        }

        ~Random() = default;

        float RandomFloatInRange(float left = 0.0f, float right = 1.0f) {
            if (left > right)
                return left;

            std::uniform_real_distribution<float> distribution(left, right);
            return distribution(mEngine);
        }

        int32 RandomIntInRange(int32 left, int32 right) {
            if (left > right)
                return left;

            std::uniform_int_distribution<int32> distribution(left, right);
            return distribution(mEngine);
        }

        int32 RandomInt() {
            std::uniform_int_distribution<int32> distribution;
            return distribution(mEngine);
        }

        int64 RandomInt64() {
            std::uniform_int_distribution<int64> distribution;
            return distribution(mEngine);
        }

    private:
        std::default_random_engine mEngine;
    };

}

#endif //BERSERK_RANDOM_H
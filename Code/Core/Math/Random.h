/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RANDOM_H
#define BERSERK_RANDOM_H

#include <Typedefs.h>
#include <Containers/TArray.h>
#include <random>

namespace Berserk {

    class Random final {
    public:
        Random() { mEngine.seed(std::time(nullptr)); };
        ~Random() = default;

        float from(float left, float right) {
            std::uniform_real_distribution<float> distribution(left, right);
            return distribution(mEngine);
        }
        int32 from(int32 left, int32 right) {
            std::uniform_int_distribution<int32> distribution(left, right);
            return distribution(mEngine);
        }
        int32 randi32() {
            std::uniform_int_distribution<int32> distribution;
            return distribution(mEngine);
        }
        int64 randi64() {
            std::uniform_int_distribution<int64> distribution;
            return distribution(mEngine);
        }
    private:
        std::default_random_engine mEngine;
    };

}

#endif //BERSERK_RANDOM_H
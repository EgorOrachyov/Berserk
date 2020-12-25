/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RANDOM_H
#define BERSERK_RANDOM_H

#include <BerserkCore/Typedefs.hpp>
#include <random>
#include <chrono>

namespace Berserk {
    namespace Math {

        /**
         * @brief Random generator
         *
         * Wraps default C++ random engine.
         * Allows to create random engine with time-based seed.
         */
        class Random final {
        public:

            explicit Random(uint64 seed)
                : mSeed(seed), mEngine(seed) {

            }

            Random() : Random(0) {}

            ~Random() = default;

            float FloatInRange(float left = 0.0f, float right = 1.0f) {
                if (left > right)
                    return left;

                std::uniform_real_distribution<float> distribution(left, right);
                return distribution(mEngine);
            }

            int32 Int32InRange(int32 left, int32 right) {
                if (left > right)
                    return left;

                std::uniform_int_distribution<int32> distribution(left, right);
                return distribution(mEngine);
            }

            int32 Int32() {
                std::uniform_int_distribution<int32> distribution(std::numeric_limits<int32>::min(), std::numeric_limits<int32>::max());
                return distribution(mEngine);
            }

            int64 Int64() {
                std::uniform_int_distribution<int64> distribution(std::numeric_limits<int64>::min(), std::numeric_limits<int64>::max());
                return distribution(mEngine);
            }

            uint32 Uint32() {
                std::uniform_int_distribution<uint32> distribution;
                return distribution(mEngine);
            }

            uint64 Uint64() {
                std::uniform_int_distribution<uint64> distribution;
                return distribution(mEngine);
            }

            static Random WithTimeSeed() {
                return Random((uint64) std::chrono::system_clock::now().time_since_epoch().count());
            }

        private:
            std::default_random_engine mEngine;
            uint64 mSeed = 0;
        };
    }
}

#endif //BERSERK_RANDOM_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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
                : mSeed(seed), mEngine(static_cast<unsigned int>(seed)) {

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
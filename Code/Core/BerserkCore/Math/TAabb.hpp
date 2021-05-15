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

#ifndef BERSERK_TAABB_H
#define BERSERK_TAABB_H

#include <BerserkCore/Math/TVecN.hpp>
//#include <BerserkCore/Containers/TArrayFixed.h>

namespace Berserk {
    namespace Math {

        template<typename T>
        class TAabb {
        public:

            TAabb() = default;

            TAabb(const TVecN<T, 3> &minv, const TVecN<T, 3> &maxv) {
                min = minv;
                max = maxv;
            }

            TAabb(const TVecN<T, 3> &center, T radius) {
                TVecN<T, 3> offset = {radius, radius, radius};
                min = center - offset;
                max = center + radius;
            }

            template<uint32 C>
            TAabb(const TArrayStatic <TVecN<T, 3>, C> &points) {
                BERSERK_COND_ERROR_RET(points.GetSize() > 0, "Passed empty points array");

                min = points[0];
                max = points[0];

                for (uint32 i = 1; i < points.GetSize(); i++) {
                    this->fit(points[i]);
                }
            }

            TAabb(const TAabb &other) = default;

            TAabb(TAabb &&other) noexcept = default;

            TAabb &operator=(const TAabb &other) = default;

            TAabb &operator=(TAabb &&other) noexcept = default;

            void fit(const TVecN<T, 3> &p) {
                min = TVecN<T, 3>::min(min, p);
                max = TVecN<T, 3>::max(max, p);
            }

            void fit(const TAabb &aabb) {
                min = TVecN<T, 3>::min(min, aabb.min);
                max = TVecN<T, 3>::max(max, aabb.max);
            }

            bool contains(const TVecN<T, 3> &p) const {
                return min <= p && p <= max;
            }

            bool contains(const TAabb &aabb) const {
                return min <= aabb.min && aabb.max <= max;
            }

            template<typename V>
            void getPoints(TArrayStatic<V, 8> &points) const {
                points.Resize(8);
                points[0] = Vec3f(min[0], min[1], min[2]);
                points[1] = Vec3f(min[0], min[1], max[2]);
                points[2] = Vec3f(min[0], max[1], min[2]);
                points[3] = Vec3f(min[0], max[1], max[2]);
                points[4] = Vec3f(max[0], min[1], min[2]);
                points[5] = Vec3f(max[0], min[1], max[2]);
                points[6] = Vec3f(max[0], max[1], min[2]);
                points[7] = Vec3f(max[0], max[1], max[2]);
            }

            TVecN<T, 3> getCenter() const {
                return (max + min) * (T) 0.5;
            }

            TVecN<T, 3> getExtent() const {
                return (max - min) * (T) 0.5;
            }

        public:

            TVecN<T, 3> min;
            TVecN<T, 3> max;

        };
    }
}

#endif //BERSERK_TAABB_H
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

#ifndef BERSERK_TAABB_HPP
#define BERSERK_TAABB_HPP

#include <core/Config.hpp>
#include <core/math/MathUtils.hpp>
#include <core/math/TVecN.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

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

    void Fit(const TVecN<T, 3> &p) {
        min = TVecN<T, 3>::min(min, p);
        max = TVecN<T, 3>::max(max, p);
    }

    void Fit(const TAabb &aabb) {
        min = TVecN<T, 3>::min(min, aabb.min);
        max = TVecN<T, 3>::max(max, aabb.max);
    }

    bool Contains(const TVecN<T, 3> &p) const {
        return min <= p && p <= max;
    }

    bool Contains(const TAabb<T> &aabb) const {
        return min <= aabb.min && aabb.max <= max;
    }

    void GetPoints(T *points) const {
        points[0] = TVecN<T, 3>(min[0], min[1], min[2]);
        points[1] = TVecN<T, 3>(min[0], min[1], max[2]);
        points[2] = TVecN<T, 3>(min[0], max[1], min[2]);
        points[3] = TVecN<T, 3>(min[0], max[1], max[2]);
        points[4] = TVecN<T, 3>(max[0], min[1], min[2]);
        points[5] = TVecN<T, 3>(max[0], min[1], max[2]);
        points[6] = TVecN<T, 3>(max[0], max[1], min[2]);
        points[7] = TVecN<T, 3>(max[0], max[1], max[2]);
    }

    TVecN<T, 3> GetCenter() const {
        return (max + min) * static_cast<T>(0.5);
    }

    TVecN<T, 3> GetExtent() const {
        return (max - min) * static_cast<T>(0.5);
    }

    const TVecN<T, 3> &GetMin() const {
        return min;
    }

    const TVecN<T, 3> &GetMax() const {
        return max;
    }

private:
    TVecN<T, 3> min{0, 0, 0};
    TVecN<T, 3> max{0, 0, 0};
};

using Aabbf = TAabb<float>;

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_TAABB_HPP

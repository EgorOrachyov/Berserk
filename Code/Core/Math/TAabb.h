/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TAABB_H
#define BERSERK_TAABB_H

#include <Typedefs.h>
#include <Math/TVecN.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {

    template <typename T>
    class TAabb {
    public:

        TAabb() = default;

        TAabb(const TVecN<T,3> &minv, const TVecN<T,3> &maxv) {
            min = minv;
            max = maxv;
        }

        TAabb(const TVecN<T,3> &center, T radius) {
            TVecN<T,3> offset = { radius, radius, radius };
            min = center - offset;
            max = center + radius;
        }

        TAabb(const TAabb& other) = default;

        TAabb(TAabb&& other) noexcept = default;

        TAabb& operator=(const TAabb& other) = default;

        TAabb& operator=(TAabb&& other) noexcept = default;

        void fit(const TVecN<T,3> &p) {
            min = TVecN<T,3>::min(min, p);
            max = TVecN<T,3>::max(max, p);
        }

        void fit(const TAabb& aabb) {
            min = TVecN<T,3>::min(min, aabb.min);
            max = TVecN<T,3>::max(max, aabb.max);
        }

        bool contains(const TVecN<T,3> &p) const {
            return min <= p && p <= max;
        }

        bool contains(const TAabb& aabb) const {
            return min <= aabb.min && aabb.max <= max;
        }

        template <typename V>
        void getPoints(const TArrayStatic<V,8> &points) const {
            points.resize(8);
            points[0] = TVecN<T,3>(min[0], min[1], min[2]);
            points[1] = TVecN<T,3>(min[0], min[1], max[2]);
            points[2] = TVecN<T,3>(min[0], max[1], min[2]);
            points[3] = TVecN<T,3>(min[0], max[1], max[2]);
            points[4] = TVecN<T,3>(max[0], min[1], min[2]);
            points[5] = TVecN<T,3>(max[0], min[1], max[2]);
            points[6] = TVecN<T,3>(max[0], max[1], min[2]);
            points[7] = TVecN<T,3>(max[0], max[1], max[2]);
        }

        TVecN<T,3> getCenter() const {
            return (max + min) * (T)0.5;
        }

        TVecN<T,3> getExtent() const {
            return (max - min) * (T)0.5;
        }

    public:

        TVecN<T,3> min;
        TVecN<T,3> max;

    };

}

#endif //BERSERK_TAABB_H
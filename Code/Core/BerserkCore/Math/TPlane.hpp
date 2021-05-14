/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TPLANE_H
#define BERSERK_TPLANE_H

#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TAabb.hpp>

namespace Berserk {
    namespace Math {

        template<typename T>
        class TPlane {
        public:

            TPlane() = default;
            TPlane(const TPlane &other) = default;
            TPlane(TPlane &&other) noexcept = default;

            /** Build plane from some point and normal vector to the plane */
            TPlane(const TVecN<T, 3> &point, const TVecN<T, 3> &normal) {
                norm = normal.normalized();
                d = -TVecN<T, 3>::dot(norm, point);
            }

            /** Build plane from dots with positive side formed by counter-clock-wise points order */
            TPlane(const TVecN<T, 3> &a, const TVecN<T, 3> &b, const TVecN<T, 3> &c) {
                auto v1 = b - a;
                auto v2 = c - a;

                norm = TVecN<T, 3>::cross(v1, v2).normalized();
                d = -TVecN<T, 3>::dot(norm, a);
            }

            ~TPlane() = default;

            TPlane &operator=(const TPlane &other) = default;
            TPlane &operator=(TPlane &&other) noexcept = default;

            void move(const TVecN<T, 3> &t) {
                d += -TVecN<T, 3>::dot(t, norm);
            }

            T distance(const TVecN<T, 3> &p) const {
                return TVecN<T, 3>::dot(norm, p) + d;
            }

            bool onPositiveSide(const TVecN<T, 3> &p) const {
                return distance(p) > (T) 0;
            }

            bool onNegativeSide(const TVecN<T, 3> &p) const {
                return distance(p) < (T) 0;
            }

            bool onPlane(const TVecN<T, 3> &p) const {
                return Math::abs(distance(p)) <= Math::THRESH_POINT_ON_PLANE;
            }

            bool onPositiveSideOrIntersects(const TAabb<T> &box) const {
                auto extent = box.getExtent();
                auto center = box.getCenter();

                auto dcentet = distance(center);
                auto dextent = Math::abs(TVecN<T, 3>::dot(norm, extent));

                return dcentet >= dextent;
            }

            TVecN<T, 3> getPositiveVertex(const TAabb<T> &box) const {
                auto p = box.min;
                auto m = box.max;

                if (norm[0] >= (T) 0) p.x = m.x;
                if (norm[1] >= (T) 0) p.y = m.y;
                if (norm[2] >= (T) 0) p.z = m.z;

                return p;
            }

            T getScalar() const {
                return d;
            }

            const TVecN<T, 3> &getNormal() const {
                return norm;
            }

        public:
            /** Scalar value */
            T d = 0;
            /** Defines positive plane side */
            TVecN<T, 3> norm;
        };
    }
}

#endif //BERSERK_TPLANE_H
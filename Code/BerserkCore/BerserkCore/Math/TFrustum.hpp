/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TFRTSTUM_H
#define BERSERK_TFRTSTUM_H

#include <BerserkCore/Math/TPlane.hpp>
//#include <BerserkCore/Containers/TArrayFixed.hpp>

namespace Berserk {
    namespace Math {

        enum class EFrustumPlane {
            Near = 0,
            Far = 1,
            Top = 2,
            Bottom = 3,
            Left = 4,
            Right = 5,
            Count = 6
        };

        enum class EFrustumPoint {
            NearLT = 0,
            NearLB = 1,
            NearRT = 2,
            NearRB = 3,
            FarLT = 4,
            FarLB = 5,
            FarRT = 6,
            FarRB = 7,
            Count = 8
        };

        template<typename T>
        class Frustum {
        public:

            using Vec = TVecN<T, 3>;
            using Plane = TPlane<T>;

            static const uint32 PLANES_COUNT = (uint32) EFrustumPlane::Count;
            static const uint32 POINTS_COUNT = (uint32) EFrustumPoint::Count;

            Frustum(const Vec &pos, const Vec &dir, const Vec &up, float fov, float aspect, float near, float far) {
                auto right = Vec::cross(dir, up);
                auto tg = Math::tan(fov * ((T) 0.5));

                auto h2 = near / tg;
                auto w2 = aspect * h2;

                auto H2 = far / tg;
                auto W2 = aspect * H2;

                auto nP = pos + dir * near;
                auto nR = right * w2;
                auto nT = up * h2;

                points[EFrustumPoint::NearRT] = nP + nR + nT;
                points[EFrustumPoint::NearRB] = points[EFrustumPoint::NearRT] - nT * ((T) 2);
                points[EFrustumPoint::NearLT] = points[EFrustumPoint::NearRT] - nR * ((T) 2);
                points[EFrustumPoint::NearLB] = points[EFrustumPoint::NearRB] - nR * ((T) 2);

                auto fP = pos + dir * far;
                auto fR = right * W2;
                auto fT = up * H2;

                points[EFrustumPoint::FarRT] = nP + nR + nT;
                points[EFrustumPoint::FarRB] = points[EFrustumPoint::FarRT] - nT * ((T) 2);
                points[EFrustumPoint::FarLT] = points[EFrustumPoint::FarRT] - nR * ((T) 2);
                points[EFrustumPoint::FarLB] = points[EFrustumPoint::FarRB] - nR * ((T) 2);

                createPlanesFromPoints();
            }

            bool inside(const TVecN<T, 3> &point) {
                for (auto &p: planes) {
                    if (p.onNegativeSide(point))
                        return false;
                }

                return true;
            }

            bool inside(const TAabb<T> &box) {
                for (auto &p: planes) {
                    auto v = p.getPositiveVertex(box);

                    if (p.distance(v) < (T) 0)
                        return false;
                }

                return true;
            }

            const TArrayStatic <Vec, POINTS_COUNT> &getPoints() const { return points; };

            const TArrayStatic <Plane, PLANES_COUNT> &getPlanes() const { return planes; }

        private:

            void createPlanesFromPoints() {
                auto &nearLT = points[EFrustumPoint::NearLT];
                auto &nearLB = points[EFrustumPoint::NearLB];
                auto &nearRT = points[EFrustumPoint::NearRT];
                auto &nearRB = points[EFrustumPoint::NearRB];
                auto &farLT = points[EFrustumPoint::FarLT];
                auto &farLB = points[EFrustumPoint::FarLB];
                auto &farRT = points[EFrustumPoint::FarRT];
                auto &farRB = points[EFrustumPoint::FarRB];

                auto up = nearLT - nearLB;
                auto right = nearRB - nearLB;
                auto front = Vec::cross(up, right);

                planes[EFrustumPlane::Near] = Plane(nearLB, front);
                planes[EFrustumPlane::Far] = Plane(farLB, -front);

                auto leftDir = farLT - nearLT;
                auto leftNorm = Vec::cross(leftDir, up);
                auto topNorm = Vec::cross(leftDir, right);

                planes[EFrustumPlane::Left] = Plane(nearLB, leftNorm);
                planes[EFrustumPlane::Top] = Plane(nearLB, topNorm);

                auto rightDir = farRB - nearRB;
                auto rightNorm = Vec::cross(up, rightDir);
                auto downNorm = Vec::cross(right, rightDir);

                planes[EFrustumPlane::Right] = Plane(nearRB, rightNorm);
                planes[EFrustumPlane::Bottom] = Plane(nearRB, downNorm);
            }

            TArrayStatic <Vec, POINTS_COUNT> points;
            TArrayStatic <Plane, PLANES_COUNT> planes;

        };
    }
}

#endif //BERSERK_TFRTSTUM_H

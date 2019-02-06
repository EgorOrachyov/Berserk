//
// Created by Egor Orachyov on 06.02.2019.
//

#ifndef BERSERK_FRUSTUM_H
#define BERSERK_FRUSTUM_H

#include "Public/Math/MathUtility.h"
#include "Public/Math/Plane.h"
#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/UsageDescriptors.h"

namespace Berserk
{

    class CORE_API Frustum
    {
    public:

        enum Sides
        {
            Frustum_Near = 0,
            Frustum_Far,
            Frustum_Bottom,
            Frustum_Top,
            Frustum_Left,
            Frustum_Right,

            Frustum_Sides_Count
        };

    public:

        Frustum() = default;

        Frustum(const Plane* p);

        Frustum(const Frustum& source) = default;

        /**
         * Creates view frustum from the following params
         * @param angle  Angle in radians between top and bottom view planes
         * @param aspect Ratio of the width to the height of view frustum
         * @param near   Distance to the near plane from view position
         * @param far    Distance to the far plane from view position
         * @param pos    View position
         * @param dir    View direction (must be normalized)
         * @param up     View up vector for orientation (must be normalized)
         */
        Frustum(float32 angle, float32 aspect, float32 near, float32 far,
                const Vec3f& pos, const Vec3f& dir, const Vec3f& up);

        ~Frustum() = default;

    public:

        /**
         * Updates this view frustum by the following params
         * @param angle  Angle in radians between top and bottom view planes
         * @param aspect Ratio of the width to the height of view frustum
         * @param near   Distance to the near plane from view position
         * @param far    Distance to the far plane from view position
         * @param pos    View position
         * @param dir    View direction (must be normalized)
         * @param up     View up vector for orientation (must be normalized)
         */
        void update(float32 angle, float32 aspect, float32 near, float32 far,
                    const Vec3f& pos, const Vec3f& dir, const Vec3f& up);

    public:

        bool inside(const Vec3f& a) const;

        bool inside(const AABB& a) const;

        bool inside(const Sphere& a) const;

        void inside_SIMD(Vec3f  a[4], int32 result[4]) const;

        void inside_SIMD(AABB   a[4], int32 result[4]) const;

        void inside_SIMD(Sphere a[4], int32 result[4]) const;

        const Plane* get() const { return mPlanes; }

    private:

        Plane mPlanes[Frustum_Sides_Count];

    };

    Frustum::Frustum(const Plane *p)
    {
        mPlanes[Frustum_Near]   = p[0];
        mPlanes[Frustum_Far]    = p[0];
        mPlanes[Frustum_Bottom] = p[0];
        mPlanes[Frustum_Top]    = p[0];
        mPlanes[Frustum_Left]   = p[0];
        mPlanes[Frustum_Right]  = p[0];
    }

    Frustum::Frustum(float32 angle, float32 aspect, float32 near, float32 far, const Vec3f &pos, const Vec3f &dir,
                     const Vec3f &up)
    {
        update(angle, aspect, near, far, pos, dir, up);
    }

    void Frustum::update(float32 angle, float32 aspect, float32 near, float32 far, const Vec3f &pos, const Vec3f &dir,
                         const Vec3f &up)
    {
        float32 tan = Math::tg(angle * 0.5f);

        auto farHeightHalf  = far * tan;
        auto farWidthHalf   = aspect * farHeightHalf;

        Vec3f right = Vec3f::cross(dir, up); 

        Vec3f far_norm  = -dir;
        Vec3f far_point = pos + dir * far;

        Vec3f near_norm  = dir;
        Vec3f near_point = pos + dir * near;

        Vec3f left_far_top = far_point + up * farHeightHalf - right * farWidthHalf;
        Vec3f left_far_bottom = left_far_top - up * (2.0f * farHeightHalf);
        Vec3f right_far_top = left_far_top + right * (2.0f * farWidthHalf);

        Vec3f left_ftn  = left_far_top.getNormalized();
        Vec3f left_fbn  = left_far_bottom.getNormalized();
        Vec3f right_ftn = right_far_top.getNormalized();

        Vec3f left_norm   = Vec3f::cross(left_ftn, up);
        Vec3f top_norm    = Vec3f::cross(left_ftn, right);
        Vec3f bottom_norm = Vec3f::cross(right, left_fbn);
        Vec3f right_norm  = Vec3f::cross(up, right_ftn);

        mPlanes[Frustum_Near]   = Plane(near_point, near_norm  );
        mPlanes[Frustum_Far]    = Plane(far_point,  far_norm   );
        mPlanes[Frustum_Bottom] = Plane(pos,        bottom_norm);
        mPlanes[Frustum_Top]    = Plane(pos,        top_norm   );
        mPlanes[Frustum_Left]   = Plane(pos,        left_norm  );
        mPlanes[Frustum_Right]  = Plane(pos,        right_norm );
    }

    bool Frustum::inside(const Vec3f &a) const
    {
        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            if (mPlanes[i].distance(a) <= 0.0) return false;
        }

        return true;
    }

} // namespace Berserk

#endif //BERSERK_FRUSTUM_H

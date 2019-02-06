//
// Created by Egor Orachyov on 06.02.2019.
//

#ifndef BERSERK_FRUSTUM_H
#define BERSERK_FRUSTUM_H

#include "Public/Math/MathUtility.h"
#include "Public/Math/Plane.h"
#include "Public/Math/TVector3.h"
#include "Public/Strings/StaticString.h"
#include "Public/Misc/SIMD.h"
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

        /** @result True if point in the frustum */
        bool inside(const Vec3f& a) const;

        /** @result True if box in the frustum or intersects that */
        bool inside(const AABB& a) const;

        /** @result True if sphere in the frustum or intersects that */
        bool inside(const Sphere& a) const;

        /**
         * SIMD inside point test for 4 points array
         * @param[in]  a      Pointer to the array with 4 points
         * @param[out] result Pointer to the buffer to write results
         */
        void inside_SIMD(Vec3f  a[4], float32 result[4]) const;

        void inside_SIMD(AABB   a[4], float32 result[4]) const;

        void inside_SIMD(Sphere a[4], float32 result[4]) const;

        const Plane* get() const { return mPlanes; }

    private:

        Plane mPlanes[Frustum_Sides_Count];

    };

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
            if (mPlanes[i].distance(a) < 0.0) return false;
        }

        return true;
    }

    bool Frustum::inside(const Sphere &a) const
    {
        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            if (!mPlanes[i].positive(a)) return false;
        }

        return true;
    }

    bool Frustum::inside(const AABB &a) const
    {
        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            Vec3f p = mPlanes[i].positiveVertex(a);

            if (mPlanes[i].distance(p) < 0.0) return false;
        }

        return true;
    }

    void Frustum::inside_SIMD(Vec3f a[4], float32 result[4]) const
    {
        SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET(0xffffffff,0xffffffff,0xffffffff,0xffffffff);
        SIMD4_FLOAT32 compare = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 vx = SIMD4_FLOAT32_SET(a[0].x, a[1].x, a[2].x, a[3].x);
        SIMD4_FLOAT32 vy = SIMD4_FLOAT32_SET(a[0].y, a[1].y, a[2].y, a[3].y);
        SIMD4_FLOAT32 vz = SIMD4_FLOAT32_SET(a[0].z, a[1].z, a[2].z, a[3].z);

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            const Plane& p = mPlanes[i];
            const Vec3f& n = p.norm();
                 float32 w = p.w();

            SIMD4_FLOAT32 px = SIMD4_FLOAT32_SET(n.x, n.x, n.x, n.x);
            SIMD4_FLOAT32 py = SIMD4_FLOAT32_SET(n.y, n.y, n.y, n.y);
            SIMD4_FLOAT32 pz = SIMD4_FLOAT32_SET(n.z, n.z, n.z, n.z);
            SIMD4_FLOAT32 pw = SIMD4_FLOAT32_SET(w,   w,   w,   w  );

            SIMD4_FLOAT32 res = SIMD4_FLOAT32_MUL(vx, px);
            SIMD4_FLOAT32 tmp = SIMD4_FLOAT32_MUL(vy, py);

                          res = SIMD4_FLOAT32_ADD(res, tmp);
                          tmp = SIMD4_FLOAT32_MUL(vz,  pz);
                          res = SIMD4_FLOAT32_ADD(res, tmp);
                          res = SIMD4_FLOAT32_ADD(res, pw);

                          res = SIMD4_FLOAT32_GR_OR_EQ(res, compare);

            inside = SIMD4_FLOAT32_AND(inside, res);
        }

        SIMD4_FLOAT32_COPY(result, inside);
    }

    void Frustum::inside_SIMD(Sphere a[4], float32 result[4]) const
    {
        float32 r[] =
        {
                a[0].radius(),
                a[1].radius(),
                a[2].radius(),
                a[3].radius()
        };

        Vec3f c[] =
        {
                a[0].center(),
                a[1].center(),
                a[2].center(),
                a[3].center()
        };

        SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET(0xffffffff,0xffffffff,0xffffffff,0xffffffff);
        SIMD4_FLOAT32 compare = SIMD4_FLOAT32_SET(-r[0], -r[1], -r[2], -r[3]);

        SIMD4_FLOAT32 cx = SIMD4_FLOAT32_SET(c[0].x, c[1].x, c[2].x, c[3].x);
        SIMD4_FLOAT32 cy = SIMD4_FLOAT32_SET(c[0].y, c[1].y, c[2].y, c[3].y);
        SIMD4_FLOAT32 cz = SIMD4_FLOAT32_SET(c[0].z, c[1].z, c[2].z, c[3].z);

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            const Plane& p = mPlanes[i];
            const Vec3f& n = p.norm();
            float32 w = p.w();

            SIMD4_FLOAT32 px = SIMD4_FLOAT32_SET(n.x, n.x, n.x, n.x);
            SIMD4_FLOAT32 py = SIMD4_FLOAT32_SET(n.y, n.y, n.y, n.y);
            SIMD4_FLOAT32 pz = SIMD4_FLOAT32_SET(n.z, n.z, n.z, n.z);
            SIMD4_FLOAT32 pw = SIMD4_FLOAT32_SET(w,   w,   w,   w  );

            SIMD4_FLOAT32 res = SIMD4_FLOAT32_MUL(cx, px);
            SIMD4_FLOAT32 tmp = SIMD4_FLOAT32_MUL(cy, py);

                          res = SIMD4_FLOAT32_AND(res, tmp);
                          tmp = SIMD4_FLOAT32_MUL(cz,  pz);
                          res = SIMD4_FLOAT32_ADD(res, tmp);
                          res = SIMD4_FLOAT32_ADD(res, pw);

                          res = SIMD4_FLOAT32_GR_OR_EQ(res, compare);

            inside = SIMD4_FLOAT32_AND(inside, res);
        }

        SIMD4_FLOAT32_COPY(result, inside);
    }

} // namespace Berserk

#endif //BERSERK_FRUSTUM_H

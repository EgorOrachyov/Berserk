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

    /**
     * Camera frustum view composed from six three dimensional planes.
     * Used primary for frustum culling process to check, whether the object approximated
     * by Sphere of AABB intersects, outside or totally in the view volume to properly
     * discard its rendering.
     *
     * @note Frustum view should be updated each frame if camera movement is continuous.
     */
    class CORE_API Frustum
    {
    public:

        /** Indexing used for iterating through planes array */
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

        /**
         * SIMD inside AABB test for 4 AABB array
         * @param[in]  a      Pointer to the array with 4 AABB
         * @param[out] result Pointer to the buffer to write results
         */
        void inside_SIMD(AABB   a[4], float32 result[4]) const;

        /**
         * SIMD inside Sphere test for 4 Sphere array
         * @param[in]  a      Pointer to the array with 4 Sphere
         * @param[out] result Pointer to the buffer to write results
         */
        void inside_SIMD(Sphere a[4], float32 result[4]) const;

        /** @retirn Pointer to internal planes */
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
        /* Suppose that all points in the frustum */

        SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET(0xffffffff,0xffffffff,0xffffffff,0xffffffff);
        SIMD4_FLOAT32 compare = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 vx = SIMD4_FLOAT32_SET(a[0].x, a[1].x, a[2].x, a[3].x);
        SIMD4_FLOAT32 vy = SIMD4_FLOAT32_SET(a[0].y, a[1].y, a[2].y, a[3].y);
        SIMD4_FLOAT32 vz = SIMD4_FLOAT32_SET(a[0].z, a[1].z, a[2].z, a[3].z);

        SIMD4_FLOAT32 res;
        SIMD4_FLOAT32 tmp;

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            const Plane& p = mPlanes[i];
            const Vec3f& n = p.norm();
            float32 w = p.w();

            SIMD4_FLOAT32 px = SIMD4_FLOAT32_SET(n.x, n.x, n.x, n.x);
            SIMD4_FLOAT32 py = SIMD4_FLOAT32_SET(n.y, n.y, n.y, n.y);
            SIMD4_FLOAT32 pz = SIMD4_FLOAT32_SET(n.z, n.z, n.z, n.z);
            SIMD4_FLOAT32 pw = SIMD4_FLOAT32_SET(w,   w,   w,   w  );

            /* Calculate distance */

            res = SIMD4_FLOAT32_MUL(vx, px);
            tmp = SIMD4_FLOAT32_MUL(vy, py);

            res = SIMD4_FLOAT32_ADD(res, tmp);
            tmp = SIMD4_FLOAT32_MUL(vz,  pz);
            res = SIMD4_FLOAT32_ADD(res, tmp);
            res = SIMD4_FLOAT32_ADD(res, pw);

            /* Check whether the distance more or equal zero */
            /* if distance is negative then the AND operation set result in 0 */

            res = SIMD4_FLOAT32_GR_OR_EQ(res, compare);
            inside = SIMD4_FLOAT32_AND(inside, res);
        }

        /* Copy result for our inside 128 register in the result[4] array */

        SIMD4_FLOAT32_COPY(result, inside);
    }

    void Frustum::inside_SIMD(Sphere a[4], float32 result[4]) const
    {
        float32 r0 = a[0].radius(),
                r1 = a[1].radius(),
                r2 = a[2].radius(),
                r3 = a[3].radius();

        const Vec3f& c0 = a[0].center(),
                     c1 = a[1].center(),
                     c2 = a[2].center(),
                     c3 = a[3].center();

        /* Suppose that all spheres in the frustum */

        SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET(0xffffffff,0xffffffff,0xffffffff,0xffffffff);
        SIMD4_FLOAT32 compare = SIMD4_FLOAT32_SET(-r0, -r1, -r2, -r3);

        SIMD4_FLOAT32 cx = SIMD4_FLOAT32_SET(c0.x, c1.x, c2.x, c3.x);
        SIMD4_FLOAT32 cy = SIMD4_FLOAT32_SET(c0.y, c1.y, c2.y, c3.y);
        SIMD4_FLOAT32 cz = SIMD4_FLOAT32_SET(c0.z, c1.z, c2.z, c3.z);

        SIMD4_FLOAT32 res;
        SIMD4_FLOAT32 tmp;

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            const Plane& p = mPlanes[i];
            const Vec3f& n = p.norm();
            float32 w = p.w();

            SIMD4_FLOAT32 px = SIMD4_FLOAT32_SET(n.x, n.x, n.x, n.x);
            SIMD4_FLOAT32 py = SIMD4_FLOAT32_SET(n.y, n.y, n.y, n.y);
            SIMD4_FLOAT32 pz = SIMD4_FLOAT32_SET(n.z, n.z, n.z, n.z);
            SIMD4_FLOAT32 pw = SIMD4_FLOAT32_SET(w,   w,   w,   w  );

            /* Calculate distance */

            res = SIMD4_FLOAT32_MUL(cx, px);
            tmp = SIMD4_FLOAT32_MUL(cy, py);

            res = SIMD4_FLOAT32_ADD(res, tmp);
            tmp = SIMD4_FLOAT32_MUL(cz,  pz);
            res = SIMD4_FLOAT32_ADD(res, tmp);
            res = SIMD4_FLOAT32_ADD(res, pw);

            /* Check whether the distance more or equal zero */
            /* if distance is negative then the AND operation set result in 0 */

            res = SIMD4_FLOAT32_GR_OR_EQ(res, compare);
            inside = SIMD4_FLOAT32_AND(inside, res);
        }

        /* Copy result for our inside 128 register in the result[4] array */

        SIMD4_FLOAT32_COPY(result, inside);
    }

    void Frustum::inside_SIMD(AABB a[4], float32 result[4]) const
    {
        const Vec3f& m0 = a[0].min(),
                     m1 = a[1].min(),
                     m2 = a[2].min(),
                     m3 = a[3].min();

        const Vec3f& M0 = a[0].max(),
                     M1 = a[1].max(),
                     M2 = a[2].max(),
                     M3 = a[3].max();

        /* Suppose that all boxes in the frustum */

        SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET(0xffffffff,0xffffffff,0xffffffff,0xffffffff);
        SIMD4_FLOAT32 compare = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 cx;  // for boxes min.x values
        SIMD4_FLOAT32 cy;  // for boxes min.y values
        SIMD4_FLOAT32 cz;  // for boxes min.z values

        SIMD4_FLOAT32 Cx;  // for boxes max.x values
        SIMD4_FLOAT32 Cy;  // for boxes max.y values
        SIMD4_FLOAT32 Cz;  // for boxes max.z values

        SIMD4_FLOAT32 res;
        SIMD4_FLOAT32 tmp;

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            const Plane& p = mPlanes[i];
            const Vec3f& n = p.norm();
            float32 w = p.w();

            SIMD4_FLOAT32 px = SIMD4_FLOAT32_SET(n.x, n.x, n.x, n.x);
            SIMD4_FLOAT32 py = SIMD4_FLOAT32_SET(n.y, n.y, n.y, n.y);
            SIMD4_FLOAT32 pz = SIMD4_FLOAT32_SET(n.z, n.z, n.z, n.z);
            SIMD4_FLOAT32 pw = SIMD4_FLOAT32_SET(w,   w,   w,   w  );

            cx = SIMD4_FLOAT32_SET(m0.x, m1.x, m2.x, m3.x);
            cy = SIMD4_FLOAT32_SET(m0.y, m1.y, m2.y, m3.y);
            cz = SIMD4_FLOAT32_SET(m0.z, m1.z, m2.z, m3.z);

            Cx = SIMD4_FLOAT32_SET(M0.x, M1.x, M2.x, M3.x);
            Cy = SIMD4_FLOAT32_SET(M0.y, M1.y, M2.y, M3.y);
            Cz = SIMD4_FLOAT32_SET(M0.z, M1.z, M2.z, M3.z);

            /* Define `positive` vertex for each plane - the nearest vertex */
            /* to the plane in the direction of the plane normal vector */

            tmp = SIMD4_FLOAT32_GR_OR_EQ(px, compare);
            res = SIMD4_FLOAT32_EQ(tmp, compare);

            Cx = SIMD4_FLOAT32_AND(tmp, Cx);
            cx = SIMD4_FLOAT32_AND(res, cx);
            cx = SIMD4_FLOAT32_ADD(cx, Cx);

            tmp = SIMD4_FLOAT32_GR_OR_EQ(py, compare);
            res = SIMD4_FLOAT32_EQ(tmp, compare);

            Cy = SIMD4_FLOAT32_AND(tmp, Cy);
            cy = SIMD4_FLOAT32_AND(res, cy);
            cy = SIMD4_FLOAT32_ADD(cy, Cy);

            tmp = SIMD4_FLOAT32_GR_OR_EQ(pz, compare);
            res = SIMD4_FLOAT32_EQ(tmp, compare);

            Cz = SIMD4_FLOAT32_AND(tmp, Cz);
            cz = SIMD4_FLOAT32_AND(res, cz);
            cz = SIMD4_FLOAT32_ADD(cz, Cz);

            /* Calculate distance */

            res = SIMD4_FLOAT32_MUL(cx, px);
            tmp = SIMD4_FLOAT32_MUL(cy, py);

            res = SIMD4_FLOAT32_ADD(res, tmp);
            tmp = SIMD4_FLOAT32_MUL(cz,  pz);
            res = SIMD4_FLOAT32_ADD(res, tmp);
            res = SIMD4_FLOAT32_ADD(res, pw);

            /* Check whether the distance more or equal zero */
            /* if distance is negative then the AND operation set result in 0 */

            res = SIMD4_FLOAT32_GR_OR_EQ(res, compare);
            inside = SIMD4_FLOAT32_AND(inside, res);
        }

        /* Copy result for our inside 128 register in the result[4] array */

        SIMD4_FLOAT32_COPY(result, inside);
    }

} // namespace Berserk

#endif //BERSERK_FRUSTUM_H

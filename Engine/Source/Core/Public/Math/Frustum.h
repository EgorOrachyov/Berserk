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

        /** @return True if point in the frustum */
        inline bool inside(const Vec3f& a) const;

        /** @return True if box in the frustum or intersects that */
        inline bool inside(const AABB& a) const;

        /** @return True if sphere in the frustum or intersects that */
        inline bool inside(const Sphere& a) const;

        /**
         * SIMD inside point test for num points array
         * @param[in]  a      Pointer to the array with num points
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        inline void inside_SIMD(Vec4f* a, float32* result, uint32 num) const;

        /**
         * SIMD inside AABB test for num AABB array
         * @param[in]  a      Pointer to the array with num AABB
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        inline void inside_SIMD(AABB* a, float32* result, uint32 num) const;

        /**
         * SIMD inside Sphere test for num Sphere array
         * @param[in]  a      Pointer to the array with num Sphere
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        inline void inside_SIMD(Sphere* a, float32* result, uint32 num) const;

        /** @return Pointer to internal planes */
        inline const Plane* get() const { return mPlanes; }

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

    void Frustum::inside_SIMD(Vec4f* a, float32* result, uint32 num) const
    {
        SIMD4_FLOAT32 zero = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 planes_x[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_y[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_z[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_w[Frustum_Sides_Count];

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            planes_x[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.x);
            planes_y[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.y);
            planes_z[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.z);
            planes_w[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mW);
        }

        for (uint32 i = 0; i <= num - 4; i += 4)
        {
            /* Suppose that all points in the frustum */

            SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET1(0xffffffff);

            SIMD4_FLOAT32 v_x = SIMD4_FLOAT32_LOAD((float32*)&a[i]);
            SIMD4_FLOAT32 v_y = SIMD4_FLOAT32_LOAD((float32*)&a[i + 1]);
            SIMD4_FLOAT32 v_z = SIMD4_FLOAT32_LOAD((float32*)&a[i + 2]);
            SIMD4_FLOAT32 v_t = SIMD4_FLOAT32_LOAD((float32*)&a[i + 3]);

            SIMD4_FLOAT32_TRANSPOSE(v_x, v_y, v_z, v_t);

            for (uint32 j = 0; j < Frustum_Sides_Count; j++)
            {
                /* Calculate distance */

                SIMD4_FLOAT32 dot_x = SIMD4_FLOAT32_MUL(v_x, planes_x[j]);
                SIMD4_FLOAT32 dot_y = SIMD4_FLOAT32_MUL(v_y, planes_y[j]);
                SIMD4_FLOAT32 dot_z = SIMD4_FLOAT32_MUL(v_z, planes_z[j]);

                SIMD4_FLOAT32 sum_xy = SIMD4_FLOAT32_ADD(dot_x, dot_y);
                SIMD4_FLOAT32 sum_zw = SIMD4_FLOAT32_ADD(dot_z, planes_w[j]);

                SIMD4_FLOAT32 distance = SIMD4_FLOAT32_ADD(sum_xy, sum_zw);

                /* Check whether the distance more or equal zero */
                /* if distance is negative then the AND operation set result in 0 */

                SIMD4_FLOAT32 res = SIMD4_FLOAT32_GR_OR_EQ(distance, zero);
                inside = SIMD4_FLOAT32_AND(inside, res);
            }

            /* Copy result for our inside 128 register in the result[4] array */

            SIMD4_FLOAT32_COPY(&result[i], inside);
        }
    }

    void Frustum::inside_SIMD(Sphere* a, float32* result, uint32 num) const
    {
        SIMD4_FLOAT32 zero = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 planes_x[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_y[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_z[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_w[Frustum_Sides_Count];

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            planes_x[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.x);
            planes_y[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.y);
            planes_z[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.z);
            planes_w[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mW);
        }

        for (uint32 i = 0; i <= num - 4; i+= 4)
        {
            /* Suppose that all spheres in the frustum */

            SIMD4_FLOAT32 inside = SIMD4_FLOAT32_SET1(0xffffffff);

            SIMD4_FLOAT32 sphere_x = SIMD4_FLOAT32_LOAD((float32*)&a[i]);
            SIMD4_FLOAT32 sphere_y = SIMD4_FLOAT32_LOAD((float32*)&a[i + 1]);
            SIMD4_FLOAT32 sphere_z = SIMD4_FLOAT32_LOAD((float32*)&a[i + 2]);
            SIMD4_FLOAT32 sphere_r = SIMD4_FLOAT32_LOAD((float32*)&a[i + 3]);

            SIMD4_FLOAT32_TRANSPOSE(sphere_x, sphere_y, sphere_z, sphere_r);
            SIMD4_FLOAT32 sphere_neg_r = SIMD4_FLOAT32_SUB(zero, sphere_r);

            for (uint32 j = 0; j < Frustum_Sides_Count; j++)
            {
                /* Calculate distance */

                SIMD4_FLOAT32 dot_x = SIMD4_FLOAT32_MUL(sphere_x, planes_x[j]);
                SIMD4_FLOAT32 dot_y = SIMD4_FLOAT32_MUL(sphere_y, planes_y[j]);
                SIMD4_FLOAT32 dot_z = SIMD4_FLOAT32_MUL(sphere_z, planes_z[j]);

                SIMD4_FLOAT32 sum_xy = SIMD4_FLOAT32_ADD(dot_x, dot_y);
                SIMD4_FLOAT32 sum_zw = SIMD4_FLOAT32_ADD(dot_z, planes_w[j]);

                SIMD4_FLOAT32 distance = SIMD4_FLOAT32_ADD(sum_xy, sum_zw);

                /* Check whether the distance more or equal zero */
                /* if distance is negative then the AND operation set result in 0 */

                SIMD4_FLOAT32 res = SIMD4_FLOAT32_GR_OR_EQ(distance, sphere_neg_r);
                inside = SIMD4_FLOAT32_AND(inside, res);
            }

            /* Copy result for our inside 128 register in the result[4] array */

            SIMD4_FLOAT32_COPY(&result[i], inside);
        }
    }

    void Frustum::inside_SIMD(AABB* a, float32* result, uint32 num) const
    {
        SIMD4_FLOAT32 zero = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 planes_x[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_y[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_z[Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_w[Frustum_Sides_Count];

        for (uint32 i = 0; i < Frustum_Sides_Count; i++)
        {
            planes_x[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.x);
            planes_y[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.y);
            planes_z[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mNorm.z);
            planes_w[i] = SIMD4_FLOAT32_SET1(mPlanes[i].mW);
        }

        for (uint32 i = 0; i <= num - 4; i += 4)
        {
            /* Suppose that all boxes in the frustum */

            SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET1(0xffffffff);

            SIMD4_FLOAT32 box_min_x = SIMD4_FLOAT32_SET(a[i].mMin.x, a[i+1].mMin.x, a[i+2].mMin.x, a[i+3].mMin.x);
            SIMD4_FLOAT32 box_min_y = SIMD4_FLOAT32_SET(a[i].mMin.y, a[i+1].mMin.y, a[i+2].mMin.y, a[i+3].mMin.y);
            SIMD4_FLOAT32 box_min_z = SIMD4_FLOAT32_SET(a[i].mMin.z, a[i+1].mMin.z, a[i+2].mMin.z, a[i+3].mMin.z);

            SIMD4_FLOAT32 box_max_x = SIMD4_FLOAT32_SET(a[i].mMax.x, a[i+1].mMax.x, a[i+2].mMax.x, a[i+3].mMax.x);
            SIMD4_FLOAT32 box_max_y = SIMD4_FLOAT32_SET(a[i].mMax.y, a[i+1].mMax.y, a[i+2].mMax.y, a[i+3].mMax.y);
            SIMD4_FLOAT32 box_max_z = SIMD4_FLOAT32_SET(a[i].mMax.z, a[i+1].mMax.z, a[i+2].mMax.z, a[i+3].mMax.z);

            for (uint32 j = 0; j < Frustum_Sides_Count; j++)
            {
                /* Calculate distance */

                SIMD4_FLOAT32 dot_min_x = SIMD4_FLOAT32_MUL(box_min_x, planes_x[j]);
                SIMD4_FLOAT32 dot_min_y = SIMD4_FLOAT32_MUL(box_min_y, planes_y[j]);
                SIMD4_FLOAT32 dot_min_z = SIMD4_FLOAT32_MUL(box_min_z, planes_z[j]);

                SIMD4_FLOAT32 dot_max_x = SIMD4_FLOAT32_MUL(box_max_x, planes_x[j]);
                SIMD4_FLOAT32 dot_max_y = SIMD4_FLOAT32_MUL(box_max_y, planes_y[j]);
                SIMD4_FLOAT32 dot_max_z = SIMD4_FLOAT32_MUL(box_max_z, planes_z[j]);

                /* Define `positive` vertex for each plane - the nearest vertex */
                /* to the plane in the direction of the plane normal vector */

                SIMD4_FLOAT32 res_x = SIMD4_FLOAT32_MAX(dot_min_x, dot_max_x);
                SIMD4_FLOAT32 res_y = SIMD4_FLOAT32_MAX(dot_min_y, dot_max_y);
                SIMD4_FLOAT32 res_z = SIMD4_FLOAT32_MAX(dot_min_z, dot_max_z);

                SIMD4_FLOAT32 sum_xy = SIMD4_FLOAT32_ADD(res_x, res_y);
                SIMD4_FLOAT32 sum_zw = SIMD4_FLOAT32_ADD(res_z, planes_w[j]);

                SIMD4_FLOAT32 distance = SIMD4_FLOAT32_ADD(sum_xy, sum_zw);

                /* Check whether the distance more or equal zero */
                /* if distance is negative then the AND operation set result in 0 */

                SIMD4_FLOAT32 res = SIMD4_FLOAT32_GR_OR_EQ(distance, zero);
                           inside = SIMD4_FLOAT32_AND(inside, res);
            }

            /* Copy result for our inside 128 register in the result[4] array */

            SIMD4_FLOAT32_COPY(&result[i], inside);
        }
    }

} // namespace Berserk

#endif //BERSERK_FRUSTUM_H

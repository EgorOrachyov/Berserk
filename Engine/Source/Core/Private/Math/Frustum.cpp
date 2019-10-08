//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Frustum.h"
#include "Math/Vec4f.h"

namespace Berserk
{

    Frustum::Frustum(float32 near, float32 far, float32 bottom, float32 top, float32 left, float32 right,
                     const Vec3f &pos, const Vec3f &dir, const Vec3f &up)
    {
        // Recalculate orthographic view
        // volume from default properties

        Vec3f right_v = Vec3f::cross(dir, up);

        auto near_norm    = dir;
        auto near_point   = pos + dir * near;

        auto far_norm     = -dir;
        auto far_point    = pos + dir * far;

        auto bottom_norm  = up;
        auto bottom_point = pos - up * bottom;

        auto top_norm     = -up;
        auto top_point    = pos + up * top;

        auto left_norm    = right_v;
        auto left_point   = pos - right_v * left;

        auto right_norm   = -right_v;
        auto right_point  = pos + right_v * right;

        mPlanes[Frustum_Near]   = Plane(near_point,   near_norm  );
        mPlanes[Frustum_Far]    = Plane(far_point,    far_norm   );
        mPlanes[Frustum_Bottom] = Plane(bottom_point, bottom_norm);
        mPlanes[Frustum_Top]    = Plane(top_point,    top_norm   );
        mPlanes[Frustum_Left]   = Plane(left_point,   left_norm  );
        mPlanes[Frustum_Right]  = Plane(right_point,  right_norm );
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

}
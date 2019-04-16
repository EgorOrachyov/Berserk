//
// Created by Egor Orachyov on 16.04.2019.
//

#include "Culling/ViewVolumeCulling.h"

namespace Berserk::Render
{

    void ViewVolumeCulling::inside(const Plane *planes, const SubmitQueueNode *data, uint32 size)
    {
        const uint32 PLANES_COUNT = 6;
        const float32 VISIBLE = 1.0f;
        const float32 NOT_VISIBLE = 0.0f;

        for (uint32 i = 0; i < size; i++)
        {
            data[i].mIsInFrustum = VISIBLE;

            for (uint32 j = 0; j < PLANES_COUNT; j++)
            {
                Vec3f p = planes[j].positiveVertex(data[i].mBoundingBox);

                if (planes[i].distance(p) < 0.0)
                {
                    data[i].mIsInFrustum = NOT_VISIBLE;
                    break;
                }
            }
        }
    }

    void ViewVolumeCulling::inside_SIMD(const Plane *planes, const SubmitQueueNode *data, uint32 size)
    {
        const float32 NOT_VISIBLE = 0.0f;

        SIMD4_FLOAT32 zero = SIMD4_FLOAT32_ZERO;

        SIMD4_FLOAT32 planes_x[Frustum::Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_y[Frustum::Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_z[Frustum::Frustum_Sides_Count];
        SIMD4_FLOAT32 planes_w[Frustum::Frustum_Sides_Count];

        for (uint32 i = 0; i < Frustum::Frustum_Sides_Count; i++)
        {
            planes_x[i] = SIMD4_FLOAT32_SET1(planes[i].mNorm.x);
            planes_y[i] = SIMD4_FLOAT32_SET1(planes[i].mNorm.y);
            planes_z[i] = SIMD4_FLOAT32_SET1(planes[i].mNorm.z);
            planes_w[i] = SIMD4_FLOAT32_SET1(planes[i].mW);
        }

        for (uint32 i = 0; i <= size - 4; i += 4)
        {
            /* Suppose that all boxes in the frustum */

            const AABB& a0 = data[i + 0].mBoundingBox;
            const AABB& a1 = data[i + 1].mBoundingBox;
            const AABB& a2 = data[i + 2].mBoundingBox;
            const AABB& a3 = data[i + 3].mBoundingBox;

            SIMD4_FLOAT32 inside  = SIMD4_FLOAT32_SET1(0xffffffff);

            SIMD4_FLOAT32 box_min_x = SIMD4_FLOAT32_SET(a0.min().x, a1.min().x, a2.min().x, a3.min().x);
            SIMD4_FLOAT32 box_min_y = SIMD4_FLOAT32_SET(a0.min().y, a1.min().y, a2.min().y, a3.min().y);
            SIMD4_FLOAT32 box_min_z = SIMD4_FLOAT32_SET(a0.min().z, a1.min().z, a2.min().z, a3.min().z);

            SIMD4_FLOAT32 box_max_x = SIMD4_FLOAT32_SET(a0.max().x, a1.max().x, a2.max().x, a3.max().x);
            SIMD4_FLOAT32 box_max_y = SIMD4_FLOAT32_SET(a0.max().y, a1.max().y, a2.max().y, a3.max().y);
            SIMD4_FLOAT32 box_max_z = SIMD4_FLOAT32_SET(a0.max().z, a1.max().z, a2.max().z, a3.max().z);

            for (uint32 j = 0; j < Frustum::Frustum_Sides_Count; j++)
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

            float32 result[4] = { NOT_VISIBLE, NOT_VISIBLE, NOT_VISIBLE, NOT_VISIBLE };

            SIMD4_FLOAT32_COPY(result, inside);

            data[i + 0].mIsInFrustum = result[0];
            data[i + 1].mIsInFrustum = result[1];
            data[i + 2].mIsInFrustum = result[2];
            data[i + 3].mIsInFrustum = result[3];
        }

        inside(planes, data + (size - (size % 4)), size % 4);
    }

} // namespace Berserk::Engine
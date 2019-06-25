//
// Created by Egor Orachyov on 06.02.2019.
//

#ifndef BERSERK_FRUSTUM_H
#define BERSERK_FRUSTUM_H

#include "Math/MathUtility.h"
#include "Math/Plane.h"
#include "Math/Vec3f.h"
#include "Misc/SIMD.h"
#include "Misc/UsageDescriptors.h"

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

        /**
         * Creates view frustum for orthographic volume
         * @param near   Distance to the near plane from view position
         * @param far    Distance to the far plane from view position
         * @param bottom Distance to the bottom plane from view position
         * @param top    Distance to the top plane from view position
         * @param left   Distance to the left plane from view position
         * @param right  Distance to the right plane from view position
         * @param pos    View position
         * @param dir    View direction (must be normalized)
         * @param up     View up vector for orientation (must be normalized)
         */
        Frustum(float32 near, float32 far, float32 bottom, float32 top, float32 left, float32 right,
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
        bool inside(const Vec3f& a) const;

        /** @return True if box in the frustum or intersects that */
        bool inside(const AABB& a) const;

        /** @return True if sphere in the frustum or intersects that */
        bool inside(const Sphere& a) const;

        /**
         * SIMD inside point test for num points array
         * @param[in]  a      Pointer to the array with num points
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        void inside_SIMD(Vec4f* a, float32* result, uint32 num) const;

        /**
         * SIMD inside AABB test for num AABB array
         * @param[in]  a      Pointer to the array with num AABB
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        void inside_SIMD(AABB* a, float32* result, uint32 num) const;

        /**
         * SIMD inside Sphere test for num Sphere array
         * @param[in]  a      Pointer to the array with num Sphere
         * @param[out] result Pointer to the buffer to write results
         * @param[in]  num    Number of object to test (must be multiple of 4)
         *
         * @warning result    Must be 16 bytes aligned pointer. If it i not,
         *                    there will be `EXC_BAD_ACCESS EXC_I386_GPFLT` exception
         */
        void inside_SIMD(Sphere* a, float32* result, uint32 num) const;

        /** @return Pointer to internal planes */
        const Plane* get() const { return mPlanes; }

    private:

        Plane mPlanes[Frustum_Sides_Count];

    };

} // namespace Berserk

#endif //BERSERK_FRUSTUM_H

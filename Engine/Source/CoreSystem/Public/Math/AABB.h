//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_AABB_H
#define BERSERK_AABB_H

#include "Math/Vec3f.h"
#include "Strings/StaticString.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Axis aligned bounding box
     */
    class CORE_EXPORT AABB
    {
    public:

        /** Creates empty box (0,0,0) (0,0,0) */
        AABB();

        AABB(const AABB& source);

        /** Create box from center point and size of the side */
        AABB(const Vec3f& center, float32 size);

        /** Min and max points of the box */
        AABB(const Vec3f& min, const Vec3f& max);

        ~AABB() = default;

    public:

        AABB& operator = (const AABB& source);

        /** Translate box on vector t */
        void operator += (const Vec3f& t);

        /** Translate box on vector t */
        void operator -= (const Vec3f& t);

        /** @return true if boxes in contact */
        bool contact(const AABB& a) const;

        /** @return true if this box contains small box */
        bool contains(const AABB& small) const;

        /** @return true if this box contains point */
        bool contains(const Vec3f& p) const;

        /** @return Box which contains this and a box */
        AABB embrace(const AABB& a) const;

        /** @return Intersection box of this and a boxex */
        AABB intersect(const AABB& a) const;

    public:

        /** @return true if box is empty */
        bool empty() const;

        /** @return Sizes ot this box  */
        const Vec3f& size() const;

        /** @return Center point of this box */
        const Vec3f& center() const;

        /** @return Min bound */
        const Vec3f& min() const;

        /** @return Max bound */
        const Vec3f& max() const;

        /** @return Box one of eight vertices [range is not checked] */
        Vec3f vertex(uint32 i) const
        {
            Vec3f v[] =
            {
                    Vec3f(mMin.x, mMin.y, mMin.z),
                    Vec3f(mMin.x, mMin.y, mMax.z),
                    Vec3f(mMin.x, mMax.y, mMin.z),
                    Vec3f(mMin.x, mMax.y, mMax.z),
                    Vec3f(mMax.x, mMin.y, mMin.z),
                    Vec3f(mMax.x, mMin.y, mMax.z),
                    Vec3f(mMax.x, mMax.y, mMin.z),
                    Vec3f(mMax.x, mMax.y, mMax.z)
            };

            return v[i];
        }

        /** @return String representation of the box */
        CName toString() const;

    public:

        /** @return true if a and b in contacts */
        static bool contact(const AABB& a, const AABB& b);

        /** @return true if big contains smaller box */
        static bool contains(const AABB& big, const AABB& small);

        /** @return Null box */
        static AABB null();

        /** @return Box which contains a and b boxes */
        static AABB embrace(const AABB& a, const AABB& b);

        /** @return Intersection of boxes a and b */
        static AABB intersect(const AABB& a, const AABB& b);

    private:

        friend class Frustum;

        Vec3f mMin;
        Vec3f mMax;

    };

} // namespace Berserk

#endif //BERSERK_AABB_H
//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_SPHERE_H
#define BERSERK_SPHERE_H

#include "Math/Vec3f.h"
#include "Strings/Old/StaticString.h"
#include "Misc/UsageDescriptors.h"

namespace Berserk
{

    /**
     * Sphere for objects bounding purposes
     */
    class CORE_EXPORT Sphere
    {
    public:

        /** With center (0,0,0) and radius (0) */
        Sphere();

        /** Copy */
        Sphere(const Sphere& source);

        /** From center point and radius */
        Sphere(const Vec3f& center, float32 radius);

        ~Sphere() = default;

    public:

        Sphere& operator = (const Sphere& source);

        /** Translate box on vector t */
        void operator += (const Vec3f& t);

        /** Translate box on vector t */
        void operator -= (const Vec3f& t);

        /** @return true if spheres in contact */
        bool contact(const Sphere& a) const;

        /** @return true if this sphere contains small one */
        bool contains(const Sphere& small) const;

        /** @return true if this sphere contains point */
        bool contains(const Vec3f& p) const;

        /** @return Sphere which contains this and a sphere */
        Sphere embrace(const Sphere& a) const;

        /** @return Intersection sphere of this and a spheres */
        Sphere intersect(const Sphere& a) const;

    public:

        /** @return true if sphere is empty */
        bool empty() const;

        /** @return Center point of this sphere */
        const Vec3f& center() const;

        /** @return Radius this sphere */
        float32 radius() const;

        /** @return String representation of the sphere */
        CName toString() const;

    public:

        /** @return true if a and b in contacts */
        static bool contact(const Sphere& a, const Sphere& b);

        /** @return true if big contains smaller sphere */
        static bool contains(const Sphere& big, const Sphere& small);

        /** @return Null sphere */
        static Sphere null();

        /** @return Sphere which contains a and b spheres */
        static Sphere embrace(const Sphere& a, const Sphere& b);

        /** @return Intersection of spheres a and b */
        static Sphere intersect(const Sphere& a, const Sphere& b);

    private:

        Vec3f mCenter;
        float32 mRadius;

    };

} // namespace Berserk

#endif //BERSERK_SPHERE_H
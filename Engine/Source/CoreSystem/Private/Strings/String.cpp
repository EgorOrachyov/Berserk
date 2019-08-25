//
// Created by Egor Orachyov on 2019-08-25.
//

#include <Strings/String.h>
#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Math/Vec4f.h>
#include <Math/Quatf.h>

namespace Berserk
{

    StringDynamic toString(int32 value)
    {
        char buffer[32];
        snprintf(buffer, 32, "%i", value);

        return StringDynamic(buffer);
    }

    StringDynamic toString(float32 value)
    {
        char buffer[32];
        snprintf(buffer, 32, "%f", value);

        return StringDynamic(buffer);
    }

    StringDynamic toString(const Vec2f& v)
    {
        char buffer[Buffers::SIZE_64];
        Printer::print(buffer, SIZE_64, "(X=%3.3f Y=%3.3f)", v.x, v.y);
        return StringDynamic(buffer);
    }

    StringDynamic toString(const Vec3f& v)
    {
        char buffer[Buffers::SIZE_64];
        Printer::print(buffer, SIZE_64, "(X=%3.3f Y=%3.3f Z=%3.3f)", v.x, v.y, v.z);
        return StringDynamic(buffer);
    }

    StringDynamic toString(const Vec4f& v)
    {
        char buffer[Buffers::SIZE_64];
        Printer::print(buffer, SIZE_64, "(X=%3.3f Y=%3.3f Z=%3.3f W=%3.3f)", v.x, v.y, v.z, v.w);
        return StringDynamic(buffer);
    }

    StringDynamic toString(const Quatf& q)
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(S=%3.3f X=%3.3f Y=%3.3f Z=%3.3f)", q.s, q.x, q.y, q.z);
        return StringDynamic(buffer);
    }

}
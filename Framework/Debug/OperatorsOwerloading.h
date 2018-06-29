//
// Created by Egor Orachyov on 29.06.2018.
//

#ifndef BERSERKENGINE_OPERATORSOWERLOADING_H
#define BERSERKENGINE_OPERATORSOWERLOADING_H

#include <cstdio>

struct Vec3d
{
public:

    Vec3d() : x(0), y(0), z(0)
    {

    }

    Vec3d(float x, float y, float z) : x(x), y(y), z(z)
    {

    }

    Vec3d operator = (const Vec3d& v)
    {
        return Vec3d(v.x, v.y, v.z);
    }

    Vec3d operator *= (const float a)
    {
        return Vec3d(a * x, a * y, a * z);
    }

public:

    float x,y,z;
};

float dotProduct(const Vec3d& v1, const Vec3d& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3d crossProduct(const Vec3d& v1, const Vec3d& v2)
{
    return Vec3d(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

void OverloadingVectorTesting()
{
    Vec3d v = crossProduct(Vec3d(1,0,0), Vec3d(0,1,0));
    printf("%f %f %f \n", v.x, v.y, v.z);
}

#endif //BERSERKENGINE_OPERATORSOWERLOADING_H

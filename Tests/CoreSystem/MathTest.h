//
// Created by Egor Orachyov on 2019-07-04.
//

#ifndef BERSERK_MATHTEST_H
#define BERSERK_MATHTEST_H

#include <Math/MathInclude.h>

using namespace Berserk;

class MathTest
{
public:

    static void MatTransformationsTest()
    {
        auto rotation = Mat4x4f::rotate(Vec3f(0,1,0), Math::radians(445.0f));
        auto rotationt = rotation.transpose();

        auto mat = rotation * rotationt;

        auto transform = Mat4x4f::rotate(Vec3f::axisY, Math::radians(90.0f));
        Mat4x4f::translate(transform, -Vec3f::axisZ);

        auto pos = Vec4f(1,0,0,1);
        auto tpos = transform * pos;

        auto invTransform = Mat4x4f::inverseTransform(transform);
        auto invtpos = invTransform * tpos;

        OutputDevice::printf("%s \n", tpos.toString().get());
        OutputDevice::printf("%s \n", invtpos.toString().get());
    }

    static void run()
    {
        MatTransformationsTest();
    }

};


#endif //BERSERK_MATHTEST_H

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Math/Math.h>
#include <Math/TVecN.h>
#include <Math/Vec3f.h>
#include <Math/Vec4f.h>
#include <Math/Point2i.h>
#include <Math/Random.h>
#include <Math/TMatMxN.h>
#include <Math/Mat4x4f.h>

#include <TestMacro.h>
#include <thread>

using namespace Berserk;

template <uint32 M, uint32 N>
void print(const TMatMxN<float32,M,N> &r) {
    for (uint32 i = 0; i < M; i++) {
        for (uint32 j = 0; j < N; j++) {
            printf("%f ", r[i][j]);
        }
        printf("\n");
    }
}

template <uint32 N>
void print(const TVecN<float32,N> &v) {
    for (uint32 j = 0; j < N; j++) {
        printf("%f ", v[j]);
    }
    printf("\n");
}

BERSERK_TEST_SECTION(Math)
{
    BERSERK_TEST(MathFunctionsRound)
    {
        float64 values[] = {
                0.1, -0.5, 10.5, 11.2, -6.6, -6.5, -0.01
        };

        for (auto v: values) {
            printf("%lf\n", Math::round(v));
            printf("%lf\n", Math::floor(v));
            printf("%lf\n", Math::ceil(v));
        }
    };

    BERSERK_TEST(MathFunctionsRadDeg)
    {
        float64 rad[] = {
                Math::PI / 2.0f,
                Math::PI / 4.0f,
                5.0 * Math::PI,
                3.0 * Math::PI / 4.0f,
                13.0f * Math::PI / 6.0f,
        };

        float64 deg[] = {
                180.0 / 2.0f,
                180.0 / 4.0f,
                5.0 * 180.0,
                3.0 * 180.0 / 4.0f,
                13.0f * 180.0 / 6.0f,
        };

        const uint32 N = sizeof(rad) / sizeof(typeof(rad[0]));

        for (uint32 i = 0; i < N; i++) {
            printf("Rad: %lf %lf\n", rad[i], Math::degToRad(deg[i]));
            printf("Deg: %lf %lf\n", deg[i], Math::radToDeg(rad[i]));

            BERSERK_EXPECT_TRUE(Math::same(rad[i], Math::degToRad(deg[i])))
            BERSERK_EXPECT_TRUE(Math::same(deg[i], Math::radToDeg(rad[i])))
        }
    };

    BERSERK_TEST(MathFunctionsInt)
    {
        int32 a[]{
                0,
                1,
                1000,
                10,
                -10,
                -100,
                400,
                12
        };

        int32 b[]{
                0,
                6,
                300,
                12,
                -2,
                -40,
                1400,
                10012
        };

        int32 t[]{
                10,
                4,
                303,
                10,
                -90,
                -30,
                14,
                -100000
        };

        const uint32 N = sizeof(a) / sizeof(typeof(a[0]));

        for (uint32 i = 0; i < N; i++) {
            auto ai = a[i];
            auto bi = b[i];
            auto ti = t[i];

            printf("a = %i b = %i t = %i\n", ai, bi, ti);

            printf("Abs: %i\n", Math::abs(ti));
            printf("Min: %i\n", Math::min(ai, bi));
            printf("Max: %i\n", Math::max(ai, bi));
            printf("Clamp: %i\n", Math::clamp(ti, ai, bi));
            printf("Between: %i\n", Math::between(ti, ai, bi));
        }
    };

    BERSERK_TEST(TVectorN)
    {
        using Vec3f = TVecN<float32, 3>;

        Vec3f a = {1, 2, 0};
        Vec3f b = {-1, 4, 9};
        Vec3f c;
        Vec3f d = {2, 0, -1};

        float32 n = 0.0f;

        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a + b;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a - b;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a * b;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a / b;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a - b * c;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c += a;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = a -= b;
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        c = Vec3f::max(a, b);
        printf("%f %f %f\n", c.x(), c.y(), c.z());

        n = c.length();
        printf("%f \n", n);

        n = c.length2();
        printf("%f \n", n);

        n = Vec3f::distance(d, b);
        printf("%f \n", n);

        n = Vec3f::distance2(d, b);
        printf("%f \n", n);

        Vec3f left = {1, 0, 0};
        Vec3f right = {0, 4, 0};

        auto dot = Vec3f::dot(left, right);
        printf("%f \n", dot);

        auto cross = Vec3f::cross(left, right);
        printf("%f %f %f\n", cross.x(), cross.y(), cross.z());

        float32 angle = Vec3f::angle(left, right);
        printf("%f \n", angle);

        float32 deg = Math::radToDeg(angle);
        printf("%f \n", deg);

        float32 t = 0.5;

        auto lerp = Vec3f::lerp(t, left, right);
        printf("%f %f %f\n", lerp.x(), lerp.y(), lerp.z());

        auto slerp = Vec3f::slerp(t, left, right);
        printf("%f %f %f\n", slerp.x(), slerp.y(), slerp.z());

        auto slerpa = Vec3f::slerp(t, angle, left, right);
        printf("%f %f %f\n", slerpa.x(), slerpa.y(), slerpa.z());

        BERSERK_EXPECT_TRUE(slerp == slerpa)
    };

    BERSERK_TEST(Vec3f)
    {
        Vec3f a = {1.0f, 4.0f};
        Vec3f b = {0.0, -0.5f, -10.0f};
        Vec3f c = Vec3f(1, 2, 3);

        auto dot = Vec3f::dot(a, b);
        printf("%f\n", dot);

        dot = Vec3f::dot(Vec3f::X_AXIS, Vec3f::X_AXIS);
        printf("%f\n", dot);

        dot = Vec3f::dot(Vec3f::X_AXIS, Vec3f::Z_AXIS);
        printf("%f\n", dot);

        auto cross = Vec3f::cross(Vec3f::Z_AXIS, Vec3f::Y_AXIS);
        printf("%f %f %f \n", cross[0], cross[1], cross[2]);

        TVecN<float32, 4> d = b;
        TVecN<float32, 2> e = b;

        printf("%f %f \n", e[0], e[1]);
        printf("%f %f %f %f \n", d[0], d[1], d[2], d[3]);
    };

    BERSERK_TEST(Point2i)
    {
        auto p = Point2i(10, 10);
        auto d = Point2i(-2, 123);
        auto pd = p + d;

        printf("%i %i \n", pd[0], pd[1]);
    };

    BERSERK_TEST(Random)
    {
        Random random;

        for (uint32 i = 0; i < 10; i++) {
            printf("%f \n", random.from(0.0f, 4.0f));
            printf("%i \n", random.from(0, 4));
        }
    };

    BERSERK_TEST(RandomInThreads)
        {
            Random random1;
            Random random2;

            auto job1 = [&](){
                for (uint32 i = 0; i < 1000; i++) {
                    printf("1 %i\n", random1.rand());
                    printf("1 %f\n", random1.from(0.0f, 1.0f));
                }
            };
            auto job2 = [&](){
                for (uint32 i = 0; i < 1000; i++) {
                    printf("2 %i\n", random2.rand());
                    printf("2 %f\n", random2.from(0.0f, 1.0f));
                }
            };

            std::thread t1(job1);
            std::thread t2(job2);

            t1.join();
            t2.join();
        };

    BERSERK_TEST(TMatMxN)
    {
        using Mat2x2f = TMatMxN<float32,2,2>;
        using Mat3x3f = TMatMxN<float32,3,3>;
        using Mat3x4f = TMatMxN<float32,3,4>;

        Mat3x3f m0 = {
                1, 2, 3,
                4, 5, 6,
                0, 1,-1
        };
        Mat3x3f m1 = {
                1, 2, 3,
                4, 5, 6,
                0, 1,-1
        };
        Mat3x4f m2 = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 1
        };
        Mat2x2f m3 = {
                100,  1,
                10,   2,
        };

        auto mul = m1 * m2; print(mul);
        auto add = m1 + 10; print(add);
        auto sub = m0 - m1; print(sub);
        auto trp = m2.transpose(); print(trp);

        auto me0 = m2.excludeColumn(0); print(me0);
        auto me1 = m2.excludeColumn(1); print(me1);
        auto me2 = m2.excludeColumn(2); print(me2);
        auto me3 = m2.excludeColumn(3); print(me3);

        auto sub2x2 = m1.submatrix<2,2>(1,1); print(sub2x2);
        auto sub2x3 = m1.submatrix<2,3>(0,0); print(sub2x3);
        auto sub3x1 = m1.submatrix<3,1>(0,1); print(sub3x1);

        auto det0 = m0.det(); printf("Det: %f\n", det0);
        auto det1 = m1.det(); printf("Det: %f\n", det1);
        auto det2 = m2.det(); printf("Det: %f\n", det2);
        auto det3 = m3.det(); printf("Det: %f\n", det3);
    };

    BERSERK_TEST(Mat4x4f)
    {
        auto transform = Mat4x4f::rotate(Vec3f::Z_AXIS, Math::degToRad(45.0f));
        Mat4x4f::translate(transform, { 10, 0, 0 });

        Vec4f point;
        point = transform * Vec4f{ 1, 0, 0, 1 };

        print(point);

        auto transposed = transform.transpose();
        print(transposed);

        float32 buffer[16];
        transform.copyTransposed(buffer);

        auto view = Mat4x4f::lookAt({0,0,1}, {1,0,-1},{0,1,0});
        auto pos = Vec4f{1,0,0};
        auto res = view * pos;
        print(res);
    };
}
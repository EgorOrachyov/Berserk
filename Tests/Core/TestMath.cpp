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
#include <Math/Color4.h>
#include <Math/TRange.h>
#include <Math/TQuat.h>
#include <Math/Quatf.h>
#include <Math/Transformf.h>

#include <TestMacro.h>
#include <thread>

using namespace Berserk;

template <uint32 M, uint32 N>
void print(const TMatMxN<float,M,N> &r) {
    for (uint32 i = 0; i < M; i++) {
        for (uint32 j = 0; j < N; j++) {
            printf("%f ", r[i][j]);
        }
        printf("\n");
    }
}

template <uint32 N>
void print(const TVecN<float,N> &v) {
    for (uint32 j = 0; j < N; j++) {
        printf("%f ", v[j]);
    }
    printf("\n");
}

void print(const Quatf& q) {
    printf("%f ", q.scalar);
    print(q.vec);
}

BERSERK_TEST_SECTION(Math)
{
    BERSERK_TEST(MathFunctionsRound)
    {
        double values[] = {
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
        double rad[] = {
                Math::PI / 2.0f,
                Math::PI / 4.0f,
                5.0 * Math::PI,
                3.0 * Math::PI / 4.0f,
                13.0f * Math::PI / 6.0f,
        };

        double deg[] = {
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
        using Vec3f = TVecN<float, 3>;

        Vec3f a = {1, 2, 0};
        Vec3f b = {-1, 4, 9};
        Vec3f c;
        Vec3f d = {2, 0, -1};

        float n = 0.0f;

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

        float angle = Vec3f::angle(left, right);
        printf("%f \n", angle);

        float deg = Math::radToDeg(angle);
        printf("%f \n", deg);

        float t = 0.5;

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

        TVecN<float, 4> d = b;
        TVecN<float, 2> e = b;

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
                    printf("1 %i\n", random1.randi32());
                    printf("1 %f\n", random1.from(0.0f, 1.0f));
                }
            };
            auto job2 = [&](){
                for (uint32 i = 0; i < 1000; i++) {
                    printf("2 %i\n", random2.randi32());
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
        using Mat2x2f = TMatMxN<float,2,2>;
        using Mat3x3f = TMatMxN<float,3,3>;
        using Mat3x4f = TMatMxN<float,3,4>;

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
        Mat4x4f::setTranslation(transform, {10, 0, 0});

        Vec4f point;
        point = transform * Vec4f{ 1, 0, 0, 1 };

        print(point);

        auto transposed = transform.transpose();
        print(transposed);

        float buffer[16];
        transform.copyTransposed(buffer);

        auto view = Mat4x4f::lookAt({0,0,1}, {1,0,-1},{0,1,0});
        auto pos = Vec4f{1,0,0};
        auto res = view * pos;
        print(res);
    };

    BERSERK_TEST(Color4f)
    {
        auto a = Color4f(210u, 40u, 90u);
        auto b = a.toR8G8B8A8();
        auto c = a.toA8B8G8R8();

        printf("#%x #%x\n", b, c);

        auto d = a.toSRGB().toA8R8G8B8();
        auto e = a.toSRGB().toLinear().toA8R8G8B8();
        auto f = a.toA8R8G8B8();

        printf("#%x #%x #%x\n", f, d, e);

        auto cd = Color4f::fromA8R8G8B8(d);
        auto cb = Color4f::fromR8G8B8A8(b);

        printf("#%x #%x\n", cd.toA8R8G8B8(), cb.toR8G8B8A8());

        auto gray = a.gray();
        auto luminance = a.luminance();
        auto lightness = a.lightness();

        printf("%f %f %f \n", gray, luminance, lightness);

        auto w = Color4f(gray);
        print(w);
    };

    BERSERK_TEST(TRange)
    {
        for (auto i: Rangei(0,10)) {
            printf("%i\n", i);
        }

        for (auto f: Rangef(0,10)) {
            printf("%f\n", f);
        }

        for (auto f: Rangef(0,10,0.5f)) {
            printf("%f\n", f);
        }
    };

    BERSERK_TEST(Perspective)
    {
        auto M = Mat4x4f::perspective(Math::QUARTER_PIf, 1.0f, 0.1, 1.0f);

        TArray<Vec3f> pos = {
                {0.0f,0.0f,-0.1f},
                {0.0f,0.0f,-0.2f},
                {0.0f,0.0f,-0.3f},
                {0.0f,0.0f,-0.4f},
                {0.0f,0.0f,-0.5f},
                {0.0f,0.0f,-0.6f},
                {0.0f,0.0f,-0.7f},
                {0.0f,0.0f,-0.8f},
                {0.0f,0.0f,-0.9f},
                {0.0f,0.0f,-1.0f}
        };

        TArray<float> depths;

        for (auto& v: pos) {
            auto r = M * Vec4f(v, 1.0f);
            r.values[2] /= r.values[3];
            auto d = r.z() * 0.5f + 0.5f;

            depths.add(d);
        }

        for (uint32 i = 0; i < depths.size(); i++) {
            print(pos[i]);
            printf("Depth: %f\n", depths[i]);
        }
    };

    BERSERK_TEST(Quatf)
    {
        Quatf r1 = Quatf(Vec3f(1,1,0), Math::degToRad(90.0f));
        Vec3f v(1,0,0);

        print(r1);
        print(v);

        Vec3f r = r1.rotate(v);
        print(r);

        Quatf r2 = Quatf(0,Math::degToRad(90.0f),Math::degToRad(45.0f));
        print(r2);

        r = r2.rotate(v);
        print(r);

        Mat4x4f m = r2.asMatrix();
        print(m);

        r = Vec3f(m * Vec4f(v,1.0f));
        print(r);

        Quatf r3 = Quatf(m);
        print(r3);

        Quatf int1 = Quatf();
        Quatf int2 = Quatf(Vec3f(0,0,1), Math::degToRad(90.0f));
        Vec3f point = Vec3f(1,0,0);
        float angle = Quatf::angle(int1, int2);

        printf("interpolation\n");
        for (auto t: Rangef(0,1,0.1)) {
            printf("t: %f\n", t);

            printf(" lerp: ");
            auto interpolated2 = Quatf::lerp(t, int1, int2).rotate(point);
            print(interpolated2);

            auto interpolated1 = Quatf::slerp(t, angle, int1, int2).rotate(point);
            printf(" slerp: ");
            print(interpolated1);
        }

        printf("look at\n");
        auto lookAt1 = Quatf::lookAt({0,1,-1}, {0,1,1});
        auto lookAt2 = Mat4x4f::lookAt({0,0,0}, {0,1,-1}, {0,1,1});

        auto compare = Quatf(lookAt2);
        print(lookAt1);
        print(compare);

        printf("face at\n");
        auto faceAt = Quatf::faceAt({1,0,1},{0,1,0});
        auto points = { Vec3f(0,0,1), Vec3f(0,1,0), Vec3f(1,0,0), Vec3f(1,0,1), Vec3f(-1,0,1), Vec3f(0,1,1), Vec3f(0,1,1) };

        for (auto p: points) {
            auto res = faceAt.rotate(p);
            printf("%f %f ", p.length(), res.length());
            print(res);
        }
    };

    BERSERK_TEST_COND(Transformf, true)
    {
        Transformf t;
        t.rotateY(Math::degToRad(45.0f)).scaleY(0.5f).translate(Vec3f(0,0,10));
        auto v = t.transform(Vec3f(1,1,0));

        print(v);

        auto m = t.toTransformMat();
        auto im = t.toInverseTransformMat();

        auto p = Vec3f(1.0f,1.0f,0.0f);
        auto p1 = Vec3f(m * Vec4f(p, 1.0f));
        auto p2 = Vec3f(im * Vec4f(p1, 1.0f));

        print(p);
        print(p1);
        print(p2);


        Transformf w;
        w.scaleX(2.0f);

        Vec3f pos(1,1,0);
        Vec3f norm(1,1,0);

        auto M = w.toTransformMat();
        auto N = w.toNormalMat();

        auto worldPos = Vec3f(M * Vec4f(pos, 1.0f));
        auto worldNorm = Vec3f(N * Vec4f(norm, 0.0f));

        print(worldPos);
        print(worldNorm);
    };

}
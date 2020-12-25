/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Math/Utils.hpp>
#include <BerserkCore/Math/Utils2d.hpp>
#include <BerserkCore/Math/Utils3d.hpp>
#include <BerserkCore/Math/Transformf.hpp>
#include <BerserkCore/Math/TQuat.hpp>
#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/Random.hpp>
#include <BerserkCore/Platform/System.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(MathFixture)

TEST_F(MathFixture, MathUtils) {

}

TEST_F(MathFixture, RadDeg) {
    auto &out = Platform::System::Out();

    double rad[] = {
            Math::Utils::PI / 2.0f,
            Math::Utils::PI / 4.0f,
            5.0 * Math::Utils::PI,
            3.0 * Math::Utils::PI / 4.0f,
            13.0f * Math::Utils::PI / 6.0f,
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
        out.WriteF("Rad: {0} {1}\n", rad[i], Math::Utils::DegToRad(deg[i]));
        out.WriteF("Deg: {0} {1}\n", deg[i], Math::Utils::RadToDeg(rad[i]));

        EXPECT_TRUE(Math::Utils::Same(rad[i], Math::Utils::DegToRad(deg[i])));
        EXPECT_TRUE(Math::Utils::Same(deg[i], Math::Utils::RadToDeg(rad[i])));
    }
}

TEST_F(MathFixture, IntFunctions) {
    auto &out = Platform::System::Out();

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

        out.WriteF("a = {0} b = {1} t = {2}\n", ai, bi, ti);

        out.WriteF("Abs: {0}\n", Math::Utils::Abs(ti));
        out.WriteF("Min: {0}\n", Math::Utils::Min(ai, bi));
        out.WriteF("Max: {0}\n", Math::Utils::Max(ai, bi));
        out.WriteF("Clamp: {0}\n", Math::Utils::Clamp(ti, ai, bi));
        out.WriteF("Between: {0}\n", Math::Utils::Between(ti, ai, bi));
    }
}

TEST_F(MathFixture, Vec) {
    auto &out = Platform::System::Out();

    using namespace Math;

    Vec3f a = {1, 2, 0};
    Vec3f b = {-1, 4, 9};
    Vec3f c;
    Vec3f d = {2, 0, -1};

    float n = 0.0f;

    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a + b;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a - b;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a * b;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a / b;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a - b * c;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c += a;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = a -= b;
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    c = Vec3f::Max(a, b);
    out.WriteF(BERSERK_TEXT("Vec: {0}\n"), c);

    n = c.Length();
    out.WriteF("N= {0}\n", n);

    n = c.Length2();
    out.WriteF("N= {0}\n", n);

    n = Vec3f::Distance(d, b);
    out.WriteF("N= {0}\n", n);

    n = Vec3f::Distance2(d, b);
    out.WriteF("N= {0}\n", n);

    Vec3f left = {1, 0, 0};
    Vec3f right = {0, 4, 0};

    auto dot = Vec3f::Dot(left, right);
    out.WriteF("Dot= {0}\n", dot);

    auto cross = Vec3f::Cross(left, right);
    out.WriteF("Cross= {0}}\n", cross);

    float angle = Vec3f::Angle(left, right);
    out.WriteF("Angle= {0}\n", angle);

    float deg = Utils::RadToDeg(angle);
    out.WriteF("Deg= {0}\n", deg);

    float t = 0.5;

    auto lerp = Vec3f::Lerp(t, left, right);
    out.WriteF("Lerp= {0}\n", lerp);

    auto slerp = Vec3f::Slerp(t, left, right);
    out.WriteF("Slerp= {0}\n", slerp);

    auto slerpa = Vec3f::Slerp(t, angle, left, right);
    out.WriteF("Slerpa= {0}\n", slerpa);
}

TEST_F(MathFixture, Random) {
    auto &out = Platform::System::Out();
    auto fmt = Formatter<>();

    Math::Random rnd1 = Math::Random(10);
    Math::Random rnd2 = Math::Random::WithTimeSeed();

    auto toGen = 10;
    auto a = -50, b = 15;

    for (auto i = 0; i < toGen; i++) {
        out.WriteFF(fmt, BERSERK_TEXT("RND[{1}]={0}"), rnd1.Int32(), i);
        out.WriteLine();
    }

    for (auto i = 0; i < toGen; i++) {
        out.WriteFF(fmt, BERSERK_TEXT("RND[{1}]={0}"), rnd2.Int32(), i);
        out.WriteLine();
    }

    for (auto i = 0; i < toGen; i++) {
        out.WriteFF(fmt, BERSERK_TEXT("RND[{1}]={0} in range [{2},{3}]"), rnd1.Int32InRange(a, b), i, a, b);
        out.WriteLine();
    }

    for (auto i = 0; i < toGen; i++) {
        out.WriteFF(fmt, BERSERK_TEXT("RND[{1}]={0} in range [{2},{3}]"), rnd2.Int32InRange(a, b), i, a, b);
        out.WriteLine();
    }
}

template<typename T, uint32 M, uint32 N>
void print(const Math::TMatMxN<T,M,N> &m) {
    auto &out = Platform::System::Out();
    auto fmt = Formatter<>();

    out.Write(BERSERK_TEXT("[\n"));

    for (uint32 i = 0; i < M; i++) {
        out.WriteFF(fmt, BERSERK_TEXT("{0}"), m.GetRow(i));
        out.WriteLine();
    }

    out.Write(BERSERK_TEXT("]\n"));
}

template<typename T, uint32 N>
void print(const Math::TVecN<T,N> &m) {
    auto &out = Platform::System::Out();
    out.WriteF(BERSERK_TEXT("{0}\n"), m);
}

template<typename T>
void print(const Math::TQuat<T> &m) {
    auto &out = Platform::System::Out();
    out.WriteF(BERSERK_TEXT("{0}\n"), m);
}

TEST_F(MathFixture, TMatMxN) {
    using namespace Math;

    auto &out = Platform::System::Out();

    using Mat2x2f = TMatMxN<float, 2, 2>;
    using Mat3x3f = TMatMxN<float, 3, 3>;
    using Mat3x4f = TMatMxN<float, 3, 4>;

    Mat3x3f m0 = Mat3x3f{
            1, 2, 3,
            4, 5, 6,
            0, 1, -1
    };
    Mat3x3f m1 = Mat3x3f{
            1, 2, 3,
            4, 5, 6,
            0, 1, -1
    };
    Mat3x4f m2 = Mat3x4f({
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 1}
    });
    Mat2x2f m3 = Mat2x2f{
            100, 1,
            10, 2,
    };

    auto mul = m1 * m2;
    print(mul);
    auto Add = m1 + 10;
    print(Add);
    auto sub = m0 - m1;
    print(sub);
    auto trp = m2.Transpose();
    print(trp);

    auto me0 = m2.ExcludeColumn(0);
    print(me0);
    auto me1 = m2.ExcludeColumn(1);
    print(me1);
    auto me2 = m2.ExcludeColumn(2);
    print(me2);
    auto me3 = m2.ExcludeColumn(3);
    print(me3);

    auto sub2x2 = m1.Submatrix<2, 2>(1, 1);
    print(sub2x2);
    auto sub2x3 = m1.Submatrix<2, 3>(0, 0);
    print(sub2x3);
    auto sub3x1 = m1.Submatrix<3, 1>(0, 1);
    print(sub3x1);

    auto det0 = m0.Det();
    out.WriteF(BERSERK_TEXT("Det: {0}\n"), det0);
    auto det1 = m1.Det();
    out.WriteF(BERSERK_TEXT("Det: {0}\n"), det1);
    auto det2 = m2.Det();
    out.WriteF(BERSERK_TEXT("Det: {0}\n"), det2);
    auto det3 = m3.Det();
    out.WriteF(BERSERK_TEXT("Det: {0}\n"), det3);
};

TEST_F(MathFixture, Mat4x4f) {
    using namespace Math;

    auto transform = Utils3d::Rotate(Vec3f(0,0,1), Utils::DegToRad(45.0f));
    Utils3d::SetTranslation(transform, {10, 0, 0});

    Vec4f point;
    point = transform * Vec4f{1, 0, 0, 1};

    print(point);

    auto transposed = transform.Transpose();
    print(transposed);

    float buffer[16];
    transform.CopyTransposed(buffer);

    auto view = Utils3d::LookAt({0, 0, 1}, {1, 0, -1}, {0, 1, 0});
    auto pos = Vec4f{1, 0, 0, 1};
    auto res = view * pos;
    print(res);
};

TEST_F(MathFixture, Perspective) {
    using namespace Math;

    auto formatter = Formatter<>();
    auto &out = Platform::System::Out();

    auto M = Utils3d::Perspective(Utils::QUARTER_PIf, 1.0f, 0.1, 1.0f);

    Array<Vec3f> pos = {
            { 0.5f,  0.0f, -0.1f},
            { 0.0f,  0.5f, -0.2f},
            { 0.1f,  0.0f, -0.3f},
            { 0.0f,  0.1f, -0.4f},
            {-0.5f,  0.0f, -0.5f},
            { 0.0f, -0.5f, -0.6f},
            { 0.0f,  0.0f, -0.7f},
            { 0.0f,  0.0f, -0.8f},
            { 0.0f,  0.0f, -0.9f},
            { 0.0f,  0.0f, -1.0f}
    };

    Array<float> depths;

    for (auto &v: pos) {
        auto r = M * Vec4f(v, 1.0f);
        r.values[2] /= r.values[3];
        auto d = r.z() * 0.5f + 0.5f;

        depths.Add(d);
    }

    for (uint32 i = 0; i < depths.GetSize(); i++) {
        out.WriteFF(formatter, "Pos: {0} Depth: {1}\n", pos[i], depths[i]);
    }
};

TEST_F(MathFixture, Quatf) {
    using namespace Math;

    auto formatter = Formatter<>();
    auto &out = Platform::System::Out();

    Quatf r1 = Quatf(Vec3f(1, 1, 0), Utils::DegToRad(90.0f));
    Vec3f v(1, 0, 0);

    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r1);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), v);

    Vec3f r = r1.Rotate(v);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r);

    Quatf r2 = Quatf(0, Utils::DegToRad(90.0f), Utils::DegToRad(45.0f));
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r2);

    r = r2.Rotate(v);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r);

    Mat4x4f m = r2.AsMatrix();
    print(m);

    r = Vec3f(m * Vec4f(v, 1.0f));
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r);

    Quatf r3 = Quatf(m);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), r3);

    Quatf int1 = Quatf();
    Quatf int2 = Quatf(Vec3f(0, 0, 1), Utils::DegToRad(90.0f));
    Vec3f point = Vec3f(1, 0, 0);
    float angle = Quatf::Angle(int1, int2);

    printf("interpolation\n");
    for (float t = 0.0f; t < 1.0f; t += 0.1f) {
        printf("t: %f\n", t);

        printf(" lerp: ");
        auto interpolated2 = Quatf::Lerp(t, int1, int2).Rotate(point);
        out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), interpolated2);

        auto interpolated1 = Quatf::Slerp(t, angle, int1, int2).Rotate(point);
        printf(" slerp: ");
        out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), interpolated1);
    }

    printf("look at\n");
    auto lookAt1 = Quatf::LookAt({0, 1, -1}, {0, 1, 1});
    auto lookAt2 = Utils3d::LookAt({0, 0, 0}, {0, 1, -1}, {0, 1, 1});

    auto compare = Quatf(lookAt2);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), lookAt1);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), compare);

    printf("face at\n");
    auto faceAt = Quatf::FaceAt({1, 0, 1}, {0, 1, 0});
    auto points = {Vec3f(0, 0, 1), Vec3f(0, 1, 0), Vec3f(1, 0, 0), Vec3f(1, 0, 1), Vec3f(-1, 0, 1), Vec3f(0, 1, 1),
                   Vec3f(0, 1, 1)};

    for (auto p: points) {
        auto res = faceAt.Rotate(p);
        printf("%f %f ", p.Length(), res.Length());
        out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), res);
    }
};

TEST_F(MathFixture, Transformf) {
    using namespace Math;

    auto formatter = Formatter<>();
    auto &out = Platform::System::Out();

    Transformf t;
        t.rotateY(Utils::DegToRad(45.0f))
        .scaleY(0.5f)
        .translate(Vec3f(0, 0, 10));

    auto v = t.transform(Vec3f(1, 1, 0));

    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), v);

    auto m = t.ToTransformMat();
    auto im = t.ToInverseTransformMat();

    auto p = Vec3f(1.0f, 1.0f, 0.0f);
    auto p1 = Vec3f(m * Vec4f(p, 1.0f));
    auto p2 = Vec3f(im * Vec4f(p1, 1.0f));

    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), p);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), p1);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), p2);

    Transformf w;
    w.scaleX(2.0f);

    Vec3f pos(1, 1, 0);
    Vec3f norm(1, 1, 0);

    auto M = w.ToTransformMat();
    auto N = w.ToNormalMat();

    auto worldPos = Vec3f(M * Vec4f(pos, 1.0f));
    auto worldNorm = Vec3f(N * Vec4f(norm, 0.0f));

    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), worldPos);
    out.WriteFF(formatter, BERSERK_TEXT("{0}\n"), worldNorm);
};

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
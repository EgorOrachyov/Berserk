//
// Created by Egor Orachyov on 21.02.2019.
//

#include "Math/Quatf.h"
#include "Math/Vec2f.h"
#include "Math/Vec4f.h"
#include "Math/Mat4x4f.h"

namespace Berserk
{

    Quatf::Quatf()
            : s(1), x(0), y(0), z(0)
    {

    }

    Quatf::Quatf(float32 s, const Vec3f &v)
            : s(s), x(v.x), y(v.y), z(v.z)
    {

    }

    Quatf::Quatf(float32 s, float32 x, float32 y, float32 z)
            : s(s), x(x), y(y), z(z)
    {

    }

    Quatf::Quatf(const Vec3f& axis, float32 angle)
    {
        Vec3f v = Vec3f::normalize(axis) * Math::sin(angle / 2);
        s = Math::cos(angle / 2);
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Quatf::Quatf(float32 roll, float32 yaw, float32 pitch)
    {
        *this = Quatf(Vec3f::axisX, roll) *
                Quatf(Vec3f::axisY, yaw) *
                Quatf(Vec3f::axisZ, pitch);
    }

    Quatf::Quatf(const Vec3f &v)
    {
        *this = Quatf(Vec3f::axisX, v.x) *
                Quatf(Vec3f::axisY, v.y) *
                Quatf(Vec3f::axisZ, v.z);
    }


    Quatf::Quatf(const Mat4x4f &M)
    {
        float32 q[4]; // notation: x[0] y[1] z[2] w[3]

        float32 trace = M.m[0] + M.m[5] + M.m[10];
        //M = M.GetTranspose();

        // Matrix 4x4 indexes
        // 0  1  2  3
        // 4  5  6  7
        // 8  9  10 11
        // 12 13 14 15

        // Check the diagonal
        if (trace > 0.0)
        {
            // positive diagonal

            float32 s = Math::sqrt(trace + 1.0f);
            q[3] = s * 0.5f;

            float32 t = 0.5f / s;
            q[0] = (M.m[9] - M.m[6]) * t;
            q[1] = (M.m[2] - M.m[8]) * t;
            q[2] = (M.m[4] - M.m[1]) * t;
        }
        else
        {
            // negative diagonal

            int32 i = 0;
            if (M.m[5] > M.m[0]) i = 1;
            if (M.m[10] > M.m[4 * i + i]) i = 2;

            static const int32 NEXT[3] = {1, 2, 0};
            int32 j = NEXT[i];
            int32 k = NEXT[j];

            auto s = (float32) Math::sqrt(M.m[i * 4 + i] - (M.m[j * 4 + j] + M.m[k * 4 + k]) + 1.0);

            float32 t;
            if (s == 0.0) t = s;
            else t = 0.5f / s;

            q[i] = s * 0.5f;
            q[3] = (M.m[k * 4 + j] - M.m[j * 4 + k]) * t;
            q[j] = (M.m[j * 4 + i] - M.m[i * 4 + j]) * t;
            q[k] = (M.m[k * 4 + i] - M.m[i * 4 + k]) * t;
        }

        s = q[3];
        x = q[0];
        y = q[1];
        z = q[2];
    }


    Quatf::Quatf(const Rotation &R)
    {

    }


    Quatf Quatf::getNormalized() const
    {
        Quatf res = *this;
        return res.normalize();
    }


    Quatf Quatf::normalize()
    {
        float32 length = Math::sqrt(s * s + x * x + y * y + z * z);
        assertion_msg(length > Math::THRESH_ZERO_NORM_SQUARED, "Quaternion length should be more than 0");

        s /= length;
        x /= length;
        y /= length;
        z /= length;

        return (*this);
    }


    Quatf Quatf::inverse() const
    {
        float32 length = Math::sqrt(s * s + x * x + y * y + z * z);
        assertion_msg(length, "Cannot inverse 0 quaternion");

        return Quatf(s / length, -x / length, -y / length, -z / length);
    }


    Quatf Quatf::conjugate() const
    {
        return Quatf(s, -x, -y, -z);
    }


    float32 Quatf::getNorm() const
    {
        return (s * s + x * x + y * y + z * z);
    }


    float32 Quatf::getLength() const
    {
        return Math::Math::sqrt(s * s + x * x + y * y + z * z);
    }


    Vec3f Quatf::getVector3() const
    {
        return Vec3f(x, y, z);
    }


    float32 Quatf::getScalar() const
    {
        return s;
    }


    Quatf Quatf::operator = (const Quatf& q)
    {
        s = q.s;
        x = q.x;
        y = q.y;
        z = q.z;

        return *this;
    }


    Quatf Quatf::operator + (const Quatf& q) const
    {
        return Quatf(s + q.s, x + q.x, y + q.y, z + q.z);
    }


    Quatf Quatf::operator - (const Quatf& q) const
    {
        return Quatf(s - q.s, x - q.x, y - q.y, z - q.z);
    }


    Quatf Quatf::operator * (const Quatf& q) const
    {
        // todo: rewrite in more simple mode

        Vec3f v = Vec3f(x, y, z);
        Vec3f w = Vec3f(q.x, q.y, q.z);

        return Quatf(s * q.s - Vec3f::dot(v, w), w * s + v * q.s + Vec3f::cross(v, w));
    }


    Quatf Quatf::operator * (float32 a) const
    {
        return Quatf(s * a, x * a, y * a, z * a);
    }


    Quatf Quatf::operator / (float32 a) const
    {
        return Quatf(s / a, x / a, y / a, z / a);
    }


    const bool Quatf::operator == (const Quatf& q) const
    {
        return (s == q.s && x == q.x && y == q.y && z == q.z);
    }


    const bool Quatf::operator >= (const Quatf& q) const
    {
        return (getNorm() >= q.getNorm());
    }


    const bool Quatf::operator <= (const Quatf& q) const
    {
        return (getNorm() <= q.getNorm());
    }


    const bool Quatf::operator > (const Quatf& q) const
    {
        return (getNorm() > q.getNorm());
    }


    const bool Quatf::operator < (const Quatf& q) const
    {
        return (getNorm() < q.getNorm());
    }


    Name Quatf::toString() const
    {
        char buffer[Buffers::SIZE_64];
        sprintf(buffer, "(S=%3.3f X=%3.3f Y=%3.3f Z=%3.3f)", s, x, y, z);
        return Name(buffer);
    }


    Vec3f Quatf::rotate(const Vec3f &v) const
    {
        Quatf m = Quatf(0, v);
        Quatf r = *this * m * conjugate();
        return r.getVector3();
    }


    Vec4f Quatf::rotate(const Vec4f &v) const
    {
        Quatf m = Quatf(0, v.x, v.y, v.z);
        Quatf r = *this * m * conjugate();
        return Vec4f(r.x, r.y, r.z, v.w);
    }


    Vec3f Quatf::unrotate(const Vec3f &v) const
    {
        Quatf m = Quatf(0, v);
        Quatf r = conjugate() * m * (*this);
        return r.getVector3();
    }


    Vec3f Quatf::getAxisX() const
    {
        return rotate(Vec3f::axisX);
    }


    Vec3f Quatf::getAxisY() const
    {
        return rotate(Vec3f::axisY);
    }


    Vec3f Quatf::getAxisZ() const
    {
        return rotate(Vec3f::axisZ);
    }


    void Quatf::getAxisAngle(Vec3f &axis, float32 &angle) const
    {
        angle = 2 * Math::arccos(s);

        const float32 S = Math::sqrt(Math::max((float32)(1.0 - s*s), (float32)0.0));

        if (S >= Math::THRESH_ZERO_NORM_SQUARED) axis = Vec3f(x / S, y / S, z / S);
        else axis = Vec3f(0.0, 1.0, 0.0);
    }


    Mat4x4f Quatf::getMatrix() const
    {
        float32 xx = x * x;
        float32 xy = x * y;
        float32 xz = x * z;
        float32 xw = x * s;

        float32 yy = y * y;
        float32 yz = y * z;
        float32 yw = y * s;

        float32 zz = z * z;
        float32 zw = z * s;

        return Mat4x4f(1 - 2 * (yy + zz), 2 * (xy - zw),     2 * (xz + yw),     0,
                       2 * (xy + zw),     1 - 2 * (xx + zz), 2 * (yz - xw),     0,
                       2 * (xz - yw),     2 * (yz + xw),     1 - 2 * (xx + yy), 0,
                       0,                 0 ,                0,                 1);
    }


    float32 Quatf::dot(Quatf q1, Quatf q2)
    {
        return q1.s * q2.s + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
    }


    float32 Quatf::angle(Quatf q1, Quatf q2)
    {
        return Math::arccos(dot(q1, q2));
    }


    Vec3f Quatf::rotate(Quatf q, Vec3f v)
    {
        return q.rotate(v);
    }


    Vec4f Quatf::rotate(Quatf q, Vec4f v)
    {
        return q.rotate(v);
    }


    Quatf Quatf::lerp(Quatf q1, Quatf q2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        return (q1 * (1 - t) + q2 * t).getNormalized();
    }


    Quatf Quatf::slerp(Quatf q1, Quatf q2, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        float32 angle = Math::arccos(dot(q1, q2));
        float32 s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }


    Quatf Quatf::slerp(Quatf q1, Quatf q2, float32 angle, float32 t)
    {
        assertion_msg(t >= 0, "Interpolation param t should be more than 0");
        assertion_msg(t <= 1, "Interpolation param t should be less than 1");

        float32 s = Math::sin(angle);

        return (q1 * (Math::sin((1 - t) * angle) / s) + q2 * (Math::sin(t * angle) / s));
    }

}
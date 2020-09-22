/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_TRANSFORMF_H
#define BERSERK_TRANSFORMF_H

#include <BerserkCore/Math/TMatMxN.hpp>
#include <BerserkCore/Math/TQuat.hpp>
#include <BerserkCore/Math/TVecN.hpp>

namespace Berserk {

    /**
     * @brief Transform class
     *
     * Represent 3D space transformation object, i.e. space
     * rotation as the quaternion, origin offset and space scale.
     *
     * The logic applied to some vertices set in the spaces as follows:
     * - 1. v = Rotate(v)
     * - 2. v = Scale(v)
     * - 3. v = Offset(v)
     *
     * Could be converted to the matrix form if required.
     */
    class Transformf {
    public:

        Transformf() = default;

        Transformf& rotateX(float angleRad) {
            return rotate(Vec3f::X_AXIS, angleRad);
        }

        Transformf& rotateY(float angleRad) {
            return rotate(Vec3f::Y_AXIS, angleRad);
        }

        Transformf& rotateZ(float angleRad) {
            return rotate(Vec3f::Z_AXIS, angleRad);
        }

        Transformf& rotate(const Vec3f& axis, float angleRad) {
            mRotation = mRotation * Quatf(axis, angleRad);
            return *this;
        }

        Transformf& scaleX(float scale) {
            mScale[INDEX_X] *= scale;
            return *this;
        }

        Transformf& scaleY(float scale) {
            mScale[INDEX_Y] *= scale;
            return *this;
        }

        Transformf& scaleZ(float scale) {
            mScale[INDEX_Z] *= scale;
            return *this;
        }

        Transformf& scale(const Vec3f& scale) {
            mScale *= scale;
            return *this;
        }

        Transformf& translate(const Vec3f& t) {
            mOffset += t;
            return *this;
        }

        Vec3f transform(const Vec3f& v) const {
            auto rotated = mRotation.rotate(v);
            auto scaled = rotated * mScale;
            auto translated = scaled + mOffset;
            return translated;
        }

        Vec3f transformAsNormal(const Vec3f& v) const {
            auto rotated = mRotation.rotate(v);
            auto scaled = rotated * (Vec3f(1.0f, 1.0f, 1.0f) / mScale);
            return scaled;
        }

        /** @return 4x4 transform matrix with embeded rotation, scale and translate operations */
        Mat4x4f toTransformMat() const {
            auto rotation = mRotation.asMatrix();
            auto scale = Mat4x4f::scale(mScale);
            auto translation = Mat4x4f::translate(mOffset);
            return translation * scale * rotation;
        }

        /**
         * @warning Suppose that rotation is normalised quternion
         * @return 4x4 inverse transform matrix, that toInverseTransformMat * toTransformMat = identity
         */
        Mat4x4f toInverseTransformMat() const {
            auto rotation = mRotation.conjugate().asMatrix();
            auto scale = Mat4x4f::scale(Vec3f(1.0f,1.0f,1.0f) / mScale);
            auto translation = Mat4x4f::translate(-mOffset);
            return rotation * scale * translation;
        }

        /** @return Normal matrix, required for proper normals transformation in the vertex shaders */
        Mat4x4f toNormalMat() const {
            auto rotation = mRotation.asMatrix();
            auto scale = Mat4x4f::scale(Vec3f(1.0f,1.0f,1.0f) / mScale);
            return scale * rotation;
        }

        const Quatf& getRotation() const { return mRotation; }

        const Vec3f& getOffset() const { return mOffset; }

        const Vec3f& getScale() const { return mScale; }

        Quatf& getRotation() { return mRotation; }

        Vec3f& getOffset() { return mOffset; }

        Vec3f& getScale() { return mScale; }

        void getBasisRotation(Vec3f& x, Vec3f& y, Vec3f& z) const {
            x = mRotation.rotate(Vec3f::X_AXIS);
            y = mRotation.rotate(Vec3f::Y_AXIS);
            z = mRotation.rotate(Vec3f::Z_AXIS);
        }

        /**
         * GL style look at view matrix
         * @param eye Position of the viewer
         * @param dir View view direction
         * @param up Up vector for proper orientation
         * @return View transformation
         */
        static Transformf lookAt(const Vec3f& eye, const Vec3f& dir, const Vec3f& up) {
            Transformf t;
            t.mOffset = -eye;

            auto Z = (-dir).normalized();
            auto X = Vec3f::cross(up, Z).normalized();
            auto Y = Vec3f::cross(Z, X);

            Mat4x4f rotation(
                X[INDEX_X], X[INDEX_Y], X[INDEX_Z], 0.0f,
                Y[INDEX_X], Y[INDEX_Y], Y[INDEX_Z], 0.0f,
                Z[INDEX_X], Z[INDEX_Y], Z[INDEX_Z], 0.0f,
                0.0f,       0.0f,       0.0f,       1.0f
            );

            t.mRotation = Quatf(rotation);

            return t;
        }

    private:

        static const uint32 INDEX_X = 0;
        static const uint32 INDEX_Y = 1;
        static const uint32 INDEX_Z = 2;

        Quatf mRotation;
        Vec3f mOffset = Vec3f(0.0f,0.0f,0.0f);
        Vec3f mScale = Vec3f(1.0f,1.0f,1.0f);

    };

}

#endif //BERSERK_TRANSFORMF_H

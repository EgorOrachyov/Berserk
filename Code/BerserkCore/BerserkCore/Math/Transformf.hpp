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
    namespace Math {

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

            Transformf &rotateX(float angleRad) {
                return Rotate(Vec3f::AxisX(), angleRad);
            }

            Transformf &rotateY(float angleRad) {
                return Rotate(Vec3f::AxisY(), angleRad);
            }

            Transformf &rotateZ(float angleRad) {
                return Rotate(Vec3f::AxisZ(), angleRad);
            }

            Transformf &Rotate(const Vec3f &axis, float angleRad) {
                mRotation = mRotation * Quatf(axis, angleRad);
                return *this;
            }

            Transformf &scaleX(float scale) {
                mScale[INDEX_X] *= scale;
                return *this;
            }

            Transformf &scaleY(float scale) {
                mScale[INDEX_Y] *= scale;
                return *this;
            }

            Transformf &scaleZ(float scale) {
                mScale[INDEX_Z] *= scale;
                return *this;
            }

            Transformf &scale(const Vec3f &scale) {
                mScale *= scale;
                return *this;
            }

            Transformf &translate(const Vec3f &t) {
                mOffset += t;
                return *this;
            }

            Vec3f transform(const Vec3f &v) const {
                auto rotated = mRotation.Rotate(v);
                auto scaled = rotated * mScale;
                auto translated = scaled + mOffset;
                return translated;
            }

            Vec3f transformAsNormal(const Vec3f &v) const {
                auto rotated = mRotation.Rotate(v);
                auto scaled = rotated * (Vec3f(1.0f, 1.0f, 1.0f) / mScale);
                return scaled;
            }

            /** @return 4x4 transform matrix with embedded rotation, scale and translate operations */
            Mat4x4f ToTransformMat() const {
                auto rotation = mRotation.AsMatrix();
                auto scale = Utils3d::scale(mScale);
                auto translation = Utils3d::Translate(mOffset);
                return translation * scale * rotation;
            }

            /**
             * @warning Suppose that rotation is normalised quaternion
             * @return 4x4 inverse transform matrix, that ToInverseTransformMat * ToTransformMat = identity
             */
            Mat4x4f ToInverseTransformMat() const {
                auto rotation = mRotation.Conjugate().AsMatrix();
                auto scale = Utils3d::scale(Vec3f(1.0f, 1.0f, 1.0f) / mScale);
                auto translation = Utils3d::Translate(-mOffset);
                return rotation * scale * translation;
            }

            /** @return Normal matrix, required for proper normals transformation in the vertex shaders */
            Mat4x4f ToNormalMat() const {
                auto rotation = mRotation.AsMatrix();
                auto scale = Utils3d::scale(Vec3f(1.0f, 1.0f, 1.0f) / mScale);
                return scale * rotation;
            }

            const Quatf &GetRotation() const { return mRotation; }
            const Vec3f &GetOffset() const { return mOffset; }
            const Vec3f &GetScale() const { return mScale; }

            Quatf &GetRotation() { return mRotation; }
            Vec3f &GetOffset() { return mOffset; }
            Vec3f &GetScale() { return mScale; }

            void GetBasisRotation(Vec3f &x, Vec3f &y, Vec3f &z) const {
                x = mRotation.Rotate(Vec3f::AxisX());
                y = mRotation.Rotate(Vec3f::AxisY());
                z = mRotation.Rotate(Vec3f::AxisY());
            }

            /**
             * GL style look at view matrix
             *
             * @param eye Position of the viewer
             * @param dir View view direction
             * @param up Up vector for proper orientation
             *
             * @return View transformation
             */
            static Transformf LookAt(const Vec3f &eye, const Vec3f &dir, const Vec3f &up) {
                Transformf t;
                t.mOffset = -eye;

                auto Z = (-dir).Normalized();
                auto X = Vec3f::Cross(up, Z).Normalized();
                auto Y = Vec3f::Cross(Z, X);

                Mat4x4f rotation(
                        X[INDEX_X], X[INDEX_Y], X[INDEX_Z], 0.0f,
                        Y[INDEX_X], Y[INDEX_Y], Y[INDEX_Z], 0.0f,
                        Z[INDEX_X], Z[INDEX_Y], Z[INDEX_Z], 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                );

                t.mRotation = Quatf(rotation);

                return t;
            }

        private:

            static const uint32 INDEX_X = 0;
            static const uint32 INDEX_Y = 1;
            static const uint32 INDEX_Z = 2;

            Quatf mRotation;
            Vec3f mOffset = Vec3f(0.0f, 0.0f, 0.0f);
            Vec3f mScale = Vec3f(1.0f, 1.0f, 1.0f);

        };
    }
}

#endif //BERSERK_TRANSFORMF_H

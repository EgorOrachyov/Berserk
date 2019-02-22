//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_ROTATION_H
#define BERSERK_ROTATION_H

#include "Math/Quatf.h"

namespace Berserk
{

    /**
     * Encapsulate some rotation logic on top of the quaternion class
     */
    class CORE_EXPORT Rotation
    {
    public:

        Rotation();

        explicit Rotation(const Quatf& q);

        explicit Rotation(const Mat4x4f& m);

        Rotation(const Rotation& r) = default;

        /**
         * Build rotation from Euler angles
         *
         * @note Angles should be in radians
         * @note Watch param description to exclude misunderstanding because of
         *       relating (roll, yaw, pitch) and rotation axises (oX,oY,oZ)
         *
         * @param roll  oX Clockwise rotation angle
         * @param yaw   oY Clockwise rotation angle
         * @param pitch oZ Clockwise rotation angle
         */
        Rotation(float32 roll, float32 yaw, float32 pitch);

        ~Rotation() = default;

    public:

        Rotation& operator = (const Rotation& r);

        /**
         * Creates new rotation, which firstly applies
         * rotation r and the rotation `this`
         * @param r Rotation
         */
        Rotation operator * (const Rotation& r);

        /**
         * Adds before `this` rotation r
         * @param r Rotation
         */
        void operator *= (const Rotation& r);

        /**
         * Adds to `this` rotation around oX
         * @param roll oX Clockwise rotation angle
         */
        void rotateX(float32 roll);

        /**
         * Adds to `this` rotation around oY
         * @param yaw oY Clockwise rotation angle
         */
        void rotateY(float32 yaw);

        /**
         * Adds to `this` rotation around oZ
         * @param pitch oZ Clockwise rotation angle
         */
        void rotateZ(float32 pitch);

    public:

        /** @return `this` to Matrix */
        Mat4x4f toMat4x4f() const;

        /** @return `this` to Quaternion */
        Quatf toQuatf() const { return mRotation; }

    private:

        Quatf mRotation;

    };

} // namespace Berserk

#endif //BERSERK_ROTATION_H
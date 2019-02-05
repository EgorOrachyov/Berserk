//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATHINCLUDE_H
#define BERSERK_MATHINCLUDE_H

#include "Public/Math/MathUtility.h"

#include "Public/Math/Degrees.h"
#include "Public/Math/Radians.h"

#include "Public/Math/Vector2.h"
#include "Public/Math/Vector3.h"
#include "Public/Math/Vector4.h"

#include "Public/Math/Matrix2x2.h"
#include "Public/Math/Matrix3x3.h"
#include "Public/Math/Matrix4x4.h"

namespace Berserk
{

    typedef Quat<float32> Quatf;

    typedef Vector2<float32> Vector2f;

    typedef Vector3<float32> Vector3f;

    typedef Vector4<float32> Vector4f;

    typedef Matrix2x2<float32> Matrix2x2f;

    typedef Matrix3x3<float32> Matrix3x3f;

    typedef Matrix4x4<float32> Matrix4x4f;

} // namespace Berserk

#endif //BERSERK_MATHINCLUDE_H

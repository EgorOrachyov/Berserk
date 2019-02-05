//
// Created by Egor Orachyov on 01.02.2019.
//

#ifndef BERSERK_MATHINCLUDE_H
#define BERSERK_MATHINCLUDE_H

#include "Public/Math/MathUtility.h"

#include "Public/Math/Degrees.h"
#include "Public/Math/Radians.h"

#include "Public/Math/TVector2.h"
#include "Public/Math/TVector3.h"
#include "Public/Math/TVector4.h"

#include "Public/Math/TMatrix2x2.h"
#include "Public/Math/TMatrix3x3.h"
#include "Public/Math/TMatrix4x4.h"

#include "Public/Math/TQuat.h"

namespace Berserk
{

    typedef TQuat<float32> Quatf;

    typedef TVector2<float32> Vector2f;

    typedef TVector3<float32> Vector3f;

    typedef TVector4<float32> Vector4f;

    typedef TMatrix2x2<float32> Matrix2x2f;

    typedef TMatrix3x3<float32> Matrix3x3f;

    typedef TMatrix4x4<float32> Matrix4x4f;

} // namespace Berserk

#endif //BERSERK_MATHINCLUDE_H

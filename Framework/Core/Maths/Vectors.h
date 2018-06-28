//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTORS_H
#define BERSERKENGINE_VECTORS_H

#include "../Essential/Types.h"

#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"

namespace Berserk
{
    Vector2d identityVector2d();

    Vector2d identityVector3d();

    Vector2d identityVector4d();

    float32 dotProduct(Vector2d v1, Vector2d v2);

    float32 dotProduct(Vector3d v1, Vector3d v2);

    float32 dotProduct(Vector4d v1, Vector4d v2);

    float32 length(Vector2d v);

    float32 length(Vector3d v);

    float32 length(Vector4d v);
    
    float32 quadLength(Vector2d v);
    
    float32 quadLength(Vector3d v);
    
    float32 quadLength(Vector4d v);

    float32 tripleProduct(Vector3d v1, Vector3d v2, Vector3d v3);

    Vector3d crossProduct(Vector3d v1, Vector3d v2);

    Vector2d multByElement(Vector2d v1, Vector2d v2);

    Vector3d multByElement(Vector3d v1, Vector3d v2);

    Vector4d multByElement(Vector4d v1, Vector4d v2);

    Vector2d normalize(Vector2d v);

    Vector3d normalize(Vector3d v);

    Vector4d normalize(Vector4d v);

    Vector2d lerp(Vector2d v1, Vector2d v2, float32 t);

    Vector3d lerp(Vector3d v1, Vector3d v2, float32 t);

    Vector4d lerp(Vector4d v1, Vector4d v2, float32 t);

    Vector2d slerp(Vector2d v1, Vector2d v2, float32 t);

    Vector3d slerp(Vector3d v1, Vector3d v2, float32 t);

    Vector4d slerp(Vector4d v1, Vector4d v2, float32 t);
    
    Vector2d slerp(Vector2d v1, Vector2d v2, float32 Y, float32 t);
    
    Vector3d slerp(Vector3d v1, Vector3d v2, float32 Y, float32 t);
    
    Vector4d slerp(Vector4d v1, Vector4d v2, float32 Y, float32 t);
    
    Vector2d smoothstep(Vector2d v1, Vector2d v2, float32 t);
    
    Vector3d smoothstep(Vector3d v1, Vector3d v2, float32 t);

    Vector4d smoothstep(Vector4d v1, Vector4d v2, float32 t);
    
    Vector2d smootherstep(Vector2d v1, Vector2d v2, float32 t);
    
    Vector3d smootherstep(Vector3d v1, Vector3d v2, float32 t);
    
    Vector4d smootherstep(Vector4d v1, Vector4d v2, float32 t);

} // namespace Berserk

#endif //BERSERKENGINE_VECTORS_H

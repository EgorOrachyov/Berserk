//
// Created by Egor Orachyov on 27.06.2018.
//

#ifndef BERSERKENGINE_VECTORS_H
#define BERSERKENGINE_VECTORS_H

#include "../Essential/Types.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Berserk
{
    Vector2 newVec2d(float32 x, float32 y);

    Vector4 newVec3d(float32 x, float32 y, float32 z);

    Vector4 newVec4d(float32 x, float32 y, float32 z, float32 w);

    float32 dotProduct(Vector2 v1, Vector2 v2);

    float32 dotProduct(Vector4 v1, Vector4 v2);

    float32 dotProduct(Vector4 v1, Vector4 v2);

    float32 trueLength(Vector2 v);

    float32 trueLength(Vector4 v);

    float32 trueLength(Vector4 v);
    
    float32 quadLength(Vector2 v);
    
    float32 quadLength(Vector4 v);
    
    float32 quadLength(Vector4 v);

    Vector4 crossProduct(Vector4 v1, Vector4 v2);

    float32 tripleProduct(Vector4 v1, Vector4 v2, Vector4 v3);

    Vector2 multByElement(Vector2 v1, Vector2 v2);

    Vector4 multByElement(Vector4 v1, Vector4 v2);

    Vector4 multByElement(Vector4 v1, Vector4 v2);

    Vector2 normalize(Vector2 v);

    Vector4 normalize(Vector4 v);

    Vector4 normalize(Vector4 v);

    Vector2 lerp(Vector2 v1, Vector2 v2, float32 t);

    Vector4 lerp(Vector4 v1, Vector4 v2, float32 t);

    Vector4 lerp(Vector4 v1, Vector4 v2, float32 t);

    Vector2 slerp(Vector2 v1, Vector2 v2, float32 t);

    Vector4 slerp(Vector4 v1, Vector4 v2, float32 t);

    Vector4 slerp(Vector4 v1, Vector4 v2, float32 t);
    
    Vector2 slerp(Vector2 v1, Vector2 v2, float32 Y, float32 t);
    
    Vector4 slerp(Vector4 v1, Vector4 v2, float32 Y, float32 t);
    
    Vector4 slerp(Vector4 v1, Vector4 v2, float32 Y, float32 t);
    
    Vector2 smoothstep(Vector2 v1, Vector2 v2, float32 t);
    
    Vector4 smoothstep(Vector4 v1, Vector4 v2, float32 t);

    Vector4 smoothstep(Vector4 v1, Vector4 v2, float32 t);
    
    Vector2 smootherstep(Vector2 v1, Vector2 v2, float32 t);
    
    Vector4 smootherstep(Vector4 v1, Vector4 v2, float32 t);
    
    Vector4 smootherstep(Vector4 v1, Vector4 v2, float32 t);

} // namespace Berserk

#endif //BERSERKENGINE_VECTORS_H

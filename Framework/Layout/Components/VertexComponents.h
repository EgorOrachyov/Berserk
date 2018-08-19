//
// Created by Egor Orachyov on 28.07.2018.
//

#ifndef BERSERKENGINE_VERTEXCOMPONENTS_H
#define BERSERKENGINE_VERTEXCOMPONENTS_H

#include "Math/Vector2f.h"
#include "Math/Vector3f.h"

namespace Berserk
{

    struct Vertex
    {
    public:

        Vector3f mPosition;
    };

    struct VertexPN
    {
    public:

        Vector3f mPosition;
        Vector3f mNormal;
    };

    struct VertexPT
    {
    public:

        Vector3f mPosition;
        Vector2f mTexCoords;
    };

    struct VertexPNT
    {
    public:

        Vector3f mPosition;
        Vector3f mNormal;
        Vector2f mTexCoords;
    };

    struct VertexPNTBT
    {
    public:

        Vector3f mPosition;
        Vector3f mNormal;
        Vector2f mTexCoords;
        Vector3f mBitangent;
        Vector3f mTangent;
    };

} // namespace Berserk

#endif //BERSERKENGINE_VERTEXCOMPONENTS_H

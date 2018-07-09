//
// Created by Egor Orachyov on 08.07.2018.
//

#ifndef BERSERKENGINE_TORUS_H
#define BERSERKENGINE_TORUS_H

#include "../Buffers/GLDataBufferPacker.h"
#include "../Buffers/GLGPUBuffer.h"

#include "../../Core/Math/Vector3f.h"
#include "../../Core/Math/UtilityVectors.h"
#include "../../Core/Containers/LinkedList.h"

namespace Berserk
{

    class Torus
    {
        struct Vertex
        {
            Vertex(Vector3f p, Vector3f n)
            {
                this->p = p;
                this->n = normalize(n);
            }

            Vector3f p;
            Vector3f n;
        };

        struct Triangle
        {
            Triangle(UINT16 a, UINT16 b, UINT16 c)
            {
                this->a = a;
                this->b = b;
                this->c = c;
            }

            UINT16 a,b,c;
        };

    public:

        Torus();
        ~Torus();

        void create(FLOAT32 radius, FLOAT32 ring_radius, UINT32 segments, UINT32 rings);
        void fill(GLGPUBuffer& buffer);
        void destroy();

    protected:

        LinkedList<Vertex> mPoints;
        LinkedList<Triangle> mPolygons;

    };

} // namespace Berserk

#endif //BERSERKENGINE_TORUS_H
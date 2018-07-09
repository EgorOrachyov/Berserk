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
            Triangle(uint16 a, uint16 b, uint16 c)
            {
                this->a = a;
                this->b = b;
                this->c = c;
            }

            uint16 a,b,c;
        };

    public:

        Torus();
        ~Torus();

        void create(float32 radius, float32 ring_radius, uint32 segments, uint32 rings);
        void fill(GLGPUBuffer& buffer);
        void destroy();

    protected:

        LinkedList<Vertex> mPoints;
        LinkedList<Triangle> mPolygons;

    };

} // namespace Berserk

#endif //BERSERKENGINE_TORUS_H
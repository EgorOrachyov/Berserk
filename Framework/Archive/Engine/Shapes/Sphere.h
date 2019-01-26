//
// Created by Egor Orachyov on 07.07.2018.
//

#ifndef BERSERKENGINE_SPHERE_H
#define BERSERKENGINE_SPHERE_H

#include "Buffers/GLDataBufferPacker.h"
#include "Buffers/GLGPUBuffer.h"

#include "Math/Vector3f.h"
#include "Containers/LinkedList.h"

namespace Berserk
{

    class Sphere
    {
        struct Vertex
        {
            Vertex(Vector3f p, Vector3f n)
            {
                this->p = p;
                this->n = n.getNormalized();
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

        Sphere();
        ~Sphere();

        void create(FLOAT32 radius, UINT32 segments, UINT32 levels);
        void fill(GLGPUBuffer& buffer);
        void destroy();

    protected:

        LinkedList<Vertex> mPoints;
        LinkedList<Triangle> mPolygons;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SPHERE_H
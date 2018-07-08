//
// Created by Egor Orachyov on 07.07.2018.
//

#ifndef BERSERKENGINE_SPHERE_H
#define BERSERKENGINE_SPHERE_H

#include "../Buffers/GLDataBufferPacker.h"
#include "../Buffers/GLElementBufferObject.h"

#include "../../Core/Math/Vector3f.h"
#include "../../Core/Math/UtilityVectors.h"
#include "../../Core/Containers/LinkedList.h"

namespace Berserk
{

    class Sphere
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

        Sphere();
        ~Sphere();

        void create(float32 radius, uint32 segments, uint32 levels);
        void fill(GLDataBufferPacker& packer, GLElementBufferObject& ebo);
        void destroy();

    protected:

        float32 mRadius;
        uint32 mSegments;
        uint32 mLevels;
        LinkedList<Vertex> mPoints;
        LinkedList<Triangle> mPolygons;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SPHERE_H
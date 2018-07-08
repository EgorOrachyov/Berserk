//
// Created by Egor Orachyov on 07.07.2018.
//

#include "Sphere.h"
#include "cmath"

namespace Berserk
{

    Sphere::Sphere()
    {

    }

    Sphere::~Sphere()
    {
        destroy();
    }

    void Sphere::create(float32 radius, uint32 segments, uint32 levels)
    {
        mPoints.init();
        mPolygons.init();

        auto vstep = (float32)(M_PI / (levels + 1));
        auto hstep = (float32)(2 * M_PI / (segments));

        auto su = (float32)(1 / segments);
        auto sv = (float32)(1 / (levels + 1));

        float32 tu = 0;
        float32 tv = 0;

        float32 a = 0;
        float32 b = 0;

        for(int i = 0; i <= levels + 1; i ++)
        {
            float32 nY = -cos(a);
            float32 pY = nY * radius;

            float32 nXZ = sin(a);
            float32 pXZ = nXZ * radius;

            for(int j = 0; j <= segments; j++)
            {
                float32 s = sin(b);
                float32 c = cos(b);

                float32 nX = nXZ * c;
                float32 nZ = nXZ * s;

                float32 pX = pXZ * c;
                float32 pZ = pXZ * s;

                mPoints.add(Vertex(Vector3f(pX, pY, pZ), Vector3f(nX, nY, nZ)));

                b += hstep;
            }

            a += vstep;
        }

        for(int i = 0; i < segments; i++)
        {
            mPolygons.add(Triangle(i, i + segments + 1, i + segments + 1 + 1));
        }

        for(int i = 1; i < levels; i++)
        {
            for(int j = i * (segments + 1); j < i * (segments + 1) + segments; j++)
            {
                mPolygons.add(Triangle(j, j + segments + 1, j + segments + 1 + 1));
                mPolygons.add(Triangle(j + segments + 1 + 1, j + 1, j));
            }
        }

        for(int i = levels * (segments + 1); i < levels * (segments + 1) + segments; i++)
        {
            mPolygons.add(Triangle(i + 1, i, i + segments + 1));
        }
    }

    void Sphere::fill(GLDataBufferPacker& packer, GLElementBufferObject& ebo)
    {
        auto v = (Vector3f*)mem_calloc(mPoints.getSize(), sizeof(Vector3f));
        auto n = (Vector3f*)mem_calloc(mPoints.getSize(), sizeof(Vector3f));
        auto i = (uint16*)mem_calloc(mPolygons.getSize(), sizeof(Triangle));

        mPoints.iterate(true);
        uint32 j = 0;
        while (mPoints.iterate())
        {
            v[j] = mPoints.getCurrent().p;
            n[j] = mPoints.getCurrent().n;

            j += 1;
        }

        mPolygons.iterate(true);
        j = 0;
        while (mPolygons.iterate())
        {
            i[j] = mPolygons.getCurrent().a; j++;
            i[j] = mPolygons.getCurrent().b; j++;
            i[j] = mPolygons.getCurrent().c; j++;
        }

        for(int j = 0; j < mPolygons.getSize(); j++)
        {
            for(int k = j * 3; k < j * 3 + 3; k++)
            printf("%u ", i[k]);

            printf("\n");
        }

        packer.addVertexData(v, mPoints.getSize(), 0, GLN_DO_NOT_USE);
        packer.addVertexData(n, mPoints.getSize(), 1, GLN_DO_NOT_USE);
        packer.pack();

        ebo.create(3 * mPolygons.getSize(), i, GLPM_TRIANGLES);

        mem_free(v);
        mem_free(n);
        mem_free(i);
    }

    void Sphere::destroy()
    {
        mPoints.empty();
        mPolygons.empty();
    }


} // namespace Berserk
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

        mRadius = radius;
        mSegments = segments;
        mLevels = levels;

        auto vstep = (float32)(M_PI / (levels + 1));
        auto hstep = (float32)(2 * M_PI / (segments));

        // build points with normals

        mPoints.add(Vertex(Vector3f(0,-radius,0), Vector3f(0,-1,0)));

        float32 alpha = 0;
        for(uint32 i = 0; i < levels; i++)
        {
            alpha += vstep;

            float32 norm_oY = -cos(alpha);

            float32 proj_oY = norm_oY * radius;

            float32 norm_oXZ = sin(alpha);

            float32 proj_oXZ = norm_oXZ * radius;

            float32 beta = 0;
            for(uint32 j = 0; j < segments; j++)
            {
                beta += hstep;

                float32 norm_oZ = sin(beta) * norm_oXZ;
                float32 norm_oX = cos(beta) * norm_oXZ;

                float32 proj_oZ = norm_oZ * radius;
                float32 proj_oX = norm_oX * radius;

                mPoints.add(Vertex(Vector3f(proj_oX, proj_oY, proj_oZ), Vector3f(norm_oX, norm_oY, norm_oZ)));
            }
        }

        mPoints.add(Vertex(Vector3f(0,radius,0), Vector3f(0,1,0)));

        // get polygon indices

        for(uint16 i = 1; i < segments; i++)
        {
            mPolygons.add(Triangle(0, i, (uint16)(i + 1)));
        }

        mPolygons.add(Triangle(0, segments, 1));

        for(auto i = 0; i < levels - 1; i++)
        {
            for(uint16 j = i * segments + 1; j < (i + 1) * segments; j++)
            {
                mPolygons.add(Triangle(j, j + segments, j + segments + 1));
                mPolygons.add(Triangle(j + segments + 1, j + 1, j));
            }

            mPolygons.add(Triangle((i + 1) * segments, (i + 1) * segments + segments, i * segments + 1 + segments));
            mPolygons.add(Triangle(i * segments + 1 + segments, i * segments + 1, (i + 1) * segments));
        }

        for(uint16 i = mPoints.getSize() - 1 - segments; i < mPoints.getSize() - 2; i++)
        {
            mPolygons.add(Triangle(i, mPoints.getSize() - 1, (uint16)(i + 1)));
        }

        mPolygons.add(Triangle(mPoints.getSize() - 2, mPoints.getSize() - 1, mPoints.getSize() - 1 - segments));

        mPoints.iterate(true);
        while (mPoints.iterate())
        {
            printf("%f %f %f\n", mPoints.getCurrent().p.x, mPoints.getCurrent().p.y, mPoints.getCurrent().p.z);
        }

        mPolygons.iterate(true);
        while (mPolygons.iterate())
        {
            printf("%u %u %u\n", mPolygons.getCurrent().a, mPolygons.getCurrent().b, mPolygons.getCurrent().c);
        }

        printf("Points %u Polygons %u \n", mPoints.getSize(), mPolygons.getSize());

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

            printf("%f %f %f  %f %f %f \n", v[j].x, v[j].y, v[j].z, n[j].x, n[j].y, n[j].z);

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
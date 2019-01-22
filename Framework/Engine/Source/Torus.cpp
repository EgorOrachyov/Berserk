//
// Created by Egor Orachyov on 08.07.2018.
//

#include "Shapes/Torus.h"
#include <cmath>
#include "Math/MathUtility.h"
#include "Math/UtilityMatrices.h"

namespace Berserk
{

    Torus::Torus()
    {

    }

    Torus::~Torus()
    {
        destroy();
    }

    void Torus::create(FLOAT32 radius, FLOAT32 ring_radius, UINT32 segments, UINT32 rings)
    {
        mPoints.init();
        mPolygons.init();

        auto hstep = (FLOAT32)(2 * M_PI / (rings));
        auto rstep = (FLOAT32)(2 * M_PI / (segments));

        // build points with normals

        Matrix4x4f rot = rotate(Vector3f(0,1,0), hstep);
        Matrix4x4f tran = translate(Vector3f(radius,0,0));

        Vertex* bring = (Vertex*)mem_calloc(segments, sizeof(Vertex));

        FLOAT32 angle = 0;
        for(int i = 0; i < segments; i++)
        {
            bring[i] = Vertex(Vector3f(cos(angle) * ring_radius ,sin(angle) * ring_radius,0), Vector3f(cos(angle), sin(angle), 0));

            Vector4f v = Vector4f(bring[i].p, 1);
            v = tran * v;
            bring[i].p = Vector3f(v);

            angle += rstep;
        }

        for(int i = 0; i < rings; i++)
        {
            for(int j = 0; j < segments; j++)
            {
                mPoints.add(bring[j]);

                Vector4f v = Vector4f(bring[j].p.x, bring[j].p.y, bring[j].p.z, 1);
                Vector4f n = Vector4f(bring[j].n.x, bring[j].n.y, bring[j].n.z, 0);

                v = rot * v;
                n = rot * n;

                bring[j].p = Vector3f(v.x, v.y, v.z);
                bring[j].n = Vector3f(n.x, n.y, n.z);
            }
        }

        // get polygon indices

        for(int i = 0; i < rings - 1; i++)
        {
            for(int j = i * segments; j < (i + 1) * segments - 1; j++)
            {
                mPolygons.add(Triangle(j, j + 1, j + segments + 1));
                mPolygons.add(Triangle(j + segments + 1, j + segments, j));
            }

            mPolygons.add(Triangle((i + 1) * segments - 1, i * segments, i * segments + segments));
            mPolygons.add(Triangle(i * segments + segments, (i + 1) * segments - 1 + segments, (i + 1) * segments - 1));
        }

        for(int i = (rings - 1) * segments; i < rings * segments - 1; i++)
        {
            mPolygons.add(Triangle(i, i + 1, (i + segments) % segments + 1));
            mPolygons.add(Triangle((i + segments) % segments + 1, (i + segments) % segments, i));
        }

        mPolygons.add(Triangle(rings * segments - 1, (rings - 1) * segments, 0));
        mPolygons.add(Triangle(0, segments - 1, rings * segments - 1));


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

    void Torus::fill(GLGPUBuffer& buffer)
    {
        auto v = (Vector3f*)mem_calloc(mPoints.getSize(), sizeof(Vector3f));
        auto n = (Vector3f*)mem_calloc(mPoints.getSize(), sizeof(Vector3f));
        auto i = (UINT16*)mem_calloc(mPolygons.getSize(), sizeof(Triangle));

        mPoints.iterate(true);
        UINT32 j = 0;
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

        GLDataBufferPacker packer;
        packer.init();
        packer.addVertexData(v, mPoints.getSize(), 0, GLN_DO_NOT_USE);
        packer.addVertexData(n, mPoints.getSize(), 1, GLN_DO_NOT_USE);
        packer.pack();

        buffer.attachData(packer);
        buffer.attachIndices(3 * mPolygons.getSize(), i, GLPM_TRIANGLES);

        mem_free(v);
        mem_free(n);
        mem_free(i);
    }

    void Torus::destroy()
    {
        mPoints.empty();
        mPolygons.empty();
    }

} // namespace Berserk
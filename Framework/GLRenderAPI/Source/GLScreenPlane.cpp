//
// Created by Egor Orachyov on 10.08.2018.
//

#include "Render/GLScreenPlane.h"

namespace Berserk
{

    void GLScreenPlane::init()
    {
        //  v3 ------------- v2
        //  |                |
        //  |                |
        //  |                |
        //  v0 ------------- v1

        UINT32 vcount = 4;
        VertexPT v[4];

        v[0].mPosition = Vector3f(-1,-1,0);
        v[0].mTexCoords = Vector2f(0,0);

        v[1].mPosition = Vector3f(1,-1,0);
        v[1].mTexCoords = Vector2f(1,0);

        v[2].mPosition = Vector3f(1,1,0);
        v[2].mTexCoords = Vector2f(1,1);

        v[3].mPosition = Vector3f(-1,1,0);
        v[3].mTexCoords = Vector2f(0,1);

        UINT32 icount = 6;
        UINT16 indices[] = {0,1,2,2,3,0};

        mBuffer.init();
        mBuffer.attachData(v, vcount);
        mBuffer.attachIndices(indices, icount);
    }

    void GLScreenPlane::use()
    {
        mBuffer.drawIndices();
    }

} // namespace Berserk
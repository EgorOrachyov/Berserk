/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_REGION2I_H
#define BERSERK_REGION2I_H

#include <Math/TVecN.h>

namespace Berserk {

    class Region2i : public TVecN<int32,4> {
    public:
        using TVecN<int32,4>::TVecN;
        using TVecN<int32,4>::operator=;
        using TVecN<int32,4>::values;

        Region2i(int32 x, int32 y, int32 w, int32 h) {
            values[0] = x;
            values[1] = y;
            values[2] = w;
            values[3] = h;
        }

        int32 getX() const { return values[0]; }
        int32 getY() const { return values[1]; }
        int32 getW() const { return values[2]; }
        int32 getH() const { return values[3]; }
    };

}

#endif //BERSERK_REGION2I_H
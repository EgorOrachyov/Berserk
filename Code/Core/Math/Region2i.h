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
#include <Math/Point2i.h>

namespace Berserk {

    /** Generic region used to represent area viewport */
    class Region2i : public TVecN<int32,4> {
    public:
        using TVecN<int32,4>::TVecN;
        using TVecN<int32,4>::operator=;
        using TVecN<int32,4>::values;

        Region2i(int32 x, int32 y, const Size2i& point2i) {
            values[0] = x;
            values[1] = y;
            values[2] = point2i[0];
            values[3] = point2i[1];
        }

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

        int32& getX() { return values[0]; }
        int32& getY() { return values[1]; }
        int32& getW() { return values[2]; }
        int32& getH() { return values[3]; }

        /** @return This region area size (width,height) */
        Size2i getArea() const { return Size2i{values[2],values[3]}; }

    };

}

#endif //BERSERK_REGION2I_H
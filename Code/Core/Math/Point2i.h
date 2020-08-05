/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_POINT2I_H
#define BERSERK_POINT2I_H

#include <Math/TVecN.h>

namespace Berserk {

    class Point2i : public TVecN<int32 , 2> {
    public:
        using TVecN<int32 , 2>::TVecN;
        using TVecN<int32, 2>::operator=;

        Point2i(const TVecN<int32, 2> &vec) : TVecN<int32, 2>(vec) { }
        Point2i(TVecN<int32, 2> &&vec) : TVecN<int32, 2>(std::move(vec)) { }

        Point2i(int32 x, int32 y) : TVecN<int32, 2>() {
            values[0] = x;
            values[1] = y;
        }

        int32& width() { return values[0]; }
        int32& height() { return values[1]; }

        int32 width() const { return values[0]; }
        int32 height() const { return values[1]; }
    };

    using Size2i = Point2i;

}

#endif //BERSERK_POINT2I_H
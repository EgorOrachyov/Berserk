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

        Point2i(int32 x, int32 y) : TVecN<int32, 2>() {
            values[0] = x;
            values[1] = y;
        }
    };

    using Size2i = Point2i;

}

#endif //BERSERK_POINT2I_H
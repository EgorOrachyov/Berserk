/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_POINT2I_H
#define BERSERK_POINT2I_H

#include <Math/TVectorN.h>

namespace Berserk {

    class Point2i : public TVectorN<int32 , 2> {
    public:
        using TVectorN<int32 , 2>::TVectorN;
        using TVectorN<int32, 2>::operator=;

        Point2i(int32 x, int32 y) : TVectorN<int32, 2>() {
            values[0] = x;
            values[1] = y;
        }
    };

}

#endif //BERSERK_POINT2I_H
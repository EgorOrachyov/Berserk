/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UTILS2D_H
#define BERSERK_UTILS2D_H

#include <BerserkCore/Math/TVecN.hpp>
#include <BerserkCore/Math/TMatMxN.hpp>

namespace Berserk {
    namespace Math {

        class Utils2d {
        public:

            static Mat2x2f IdentityMatrix() {
                return {
                    1, 0,
                    0, 1
                };
            }

            /** Counter clockwise rotation around imaginable Z axis for 2D space  */
            static Mat2x2f RotateZ(float angleRad) {
                auto s = Utils::Sin(angleRad);
                auto c = Utils::Cos(angleRad);

                return {
                    c, -s,
                    s, c
                };
            }


        };
    }
}

#endif //BERSERK_UTILS2D_H
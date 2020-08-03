/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MAT2X2F_H
#define BERSERK_MAT2X2F_H

#include <Math/TVecN.h>
#include <Math/TMatMxN.h>

namespace Berserk {

    class Mat2x2f : public TMatMxN<float,2,2> {
    public:
        using TMatMxN<float,2,2>::TMatMxN;
        using TMatMxN<float,2,2>::operator=;
        using Vec2f = TVecN<float,2>;

        Mat2x2f(float m_00, float m_01,
                float m_10, float m_11) {
            values[0] = m_00; values[1] = m_01;
            values[2] = m_10; values[3] = m_11;
        }

        static Mat2x2f identity() {
            return Mat2x2f(
                1, 0,
                0, 1
            );
        }

        /** Counter clockwise rotation around imaginable ortho axis for 2D space  */
        static Mat2x2f rotateZ(float angleRad) {
            auto s = Math::sin(angleRad);
            auto c = Math::cos(angleRad);

            return Mat2x2f(
                    c, -s,
                    s,  c
            );
        }


    };

}

#endif //BERSERK_MAT2X2F_H
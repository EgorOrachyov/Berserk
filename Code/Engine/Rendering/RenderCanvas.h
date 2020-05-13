/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RENDERCANVAS_H
#define BERSERK_RENDERCANVAS_H

#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Math/Color4.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Rendering canvas
         * Rendering surface for processing 2D primitives such as text,
         * lines, points, circles and simple 3D primitives such as boxes, spheres.
         */
        class RenderCanvas {
        public:

            // todo
            void drawLine(const Vec2f &from, const Vec2f &to, const Color4f &lineWidth) {}

            void drawLine(const Vec3f &from, const Vec3f &to, const Color4f &lineWidth) {}

            void drawSphere(const Vec3f &pos, float radius, const Color4f &lineWidth) {}

        };

    }
}

#endif //BERSERK_RENDERCANVAS_H
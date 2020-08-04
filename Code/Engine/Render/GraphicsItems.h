/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSITEMS_H
#define BERSERK_GRAPHICSITEMS_H

#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Math/Color4.h>
#include <Math/Mat2x2f.h>
#include <RenderResources/Texture2D.h>

namespace Berserk {
    namespace Render {

        /** Something, what could be placed to canvas */
        class GraphicsItem {
        public:
            virtual ~GraphicsItem() = default;

            /** Relative to the canvas position */
            Point2i position;
            /** Sort order (far objects has more zOrder values) */
            int32 zOrder = 0;
            /** Color used to modulate content */
            Color4f color;
        };

        class GraphicsTexture : public GraphicsItem {
        public:
            ~GraphicsTexture() override = default;

            /** Texture to be drawn */
            TPtrShared<Texture2D> texture;
            /*** Used to ignore pixels of this color */
            Color4f transparentColor;
            /** Region of the texture to place in the are */
            Region2i textureRect;
            /** Area to draw texture */
            Size2i areaSize;
            /** If render texture alpha blending */
            bool useAlpha = false;
            /** True if use transparent color to ignore pixels*/
            bool useTransparentColor = false;
        };

    }
}

#endif //BERSERK_GRAPHICSITEMS_H
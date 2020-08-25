/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GRAPHICSPEN_H
#define BERSERK_GRAPHICSPEN_H

#include <Math/Color4.h>
#include <Math/Point2i.h>

namespace Berserk {
    namespace Render {

        /** Store color and alpha settings for graphics drawing */
        class GraphicsPen {
        public:

            /** Set pen position */
            void setPosition(const Point2i& penPosition) { position = penPosition; }

            /** Set pen color */
            void setColor(const Color4f& penColor) { color = penColor; }

            /** Set alpha enable */
            void setUsingAlpha(bool enable) { useAlpha = enable; }

            /** Move pen on offset vector */
            void move(const Point2i& offset) { position += offset; }

            /** @return Pen color */
            const Color4f& getColor() const { return color; }

            /** @return Pen position on the graphics */
            const Point2i& getPosition() const { return position; }

            /** @return True if drawing with alpha blending enabled */
            bool isUsingAlpha() const { return useAlpha; }

        private:
            friend class Graphics;

            /** Pen color, by default is white */
            Color4f color = Color4f(1.0f,1.0f,1.0f,1.0f);

            /** Pen position on the graphics */
            Point2i position;

            /** True if draw with alpha transparency enabled */
            bool useAlpha = false;

        };

    }
}

#endif //BERSERK_GRAPHICSPEN_H
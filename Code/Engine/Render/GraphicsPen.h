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

namespace Berserk {
    namespace Render {

        /** Store color and alpha settings for graphics drawing */
        class GraphicsPen {
        public:

            /** Set pen color */
            void setColor(const Color4f& penColor) { color = penColor; }

            /** Set alpha enable */
            void setUsingAlpha(bool enable) { useAlpha = enable; }

            /** @return Pen color */
            const Color4f& getColor() const { return color; }

            /** @return True if drawing with alpha blending enabled */
            bool isUsingAlpha() const { return useAlpha; }

        private:
            friend class Graphics;

            /** Pen color, by default is white */
            Color4f color = Color4f(1.0f,1.0f,1.0f,1.0f);

            /** True if draw with alpha transparency enabled */
            bool useAlpha = false;

        };

    }
}

#endif //BERSERK_GRAPHICSPEN_H
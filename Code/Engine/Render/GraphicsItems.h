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
#include <RHI/RHIUniformSet.h>
#include <RHI/RHIUniformBuffer.h>
#include <RenderResources/Texture2D.h>
#include <RenderResources/DynamicIndexBuffer.h>
#include <RenderResources/DynamicVertexBuffer.h>

namespace Berserk {
    namespace Render {

        /** Something, what could be placed to canvas */
        class GraphicsItem {
        public:
            virtual ~GraphicsItem() = default;

            /** Color used to modulate content */
            Color4f color = Color4f(1.0f, 1.0f, 1.0f);
            /** Relative to the canvas position */
            Point2i position;
            /** Sort order (far objects has more zOrder values) */
            int32 zOrder = 0;
            /** If render uses alpha blending for this primitive */
            bool useAlpha = false;
        };

        /** 2D texture image, drawn on Graphics */
        class GraphicsTexture : public GraphicsItem {
        public:
            ~GraphicsTexture() override = default;

            /** Binding data for rendering */
            TPtrShared<RHIUniformSet> uniformBinding;
            /** Texture to be drawn */
            TPtrShared<Texture2D> texture;
            /*** Used to ignore pixels of this color */
            Color4f transparentColor;
            /** Region of the texture to place in the are */
            Region2i textureRect;
            /** Area to draw texture */
            Size2i areaSize;
            /** True if use transparent color to ignore pixels*/
            bool useTransparentColor = false;
            /** True if image in srgb space and must be converted to linear in draw time */
            bool isSRGB = false;
        };

        /**
         * @brief 2D primitive geometry
         * To pass geometry inside shader we use the following attributes:
         * - Position (in the canvas)
         * - Color (rgb of the vertex)
         */
        class GraphicsPrimitive : public GraphicsItem {
        public:
            /**
             * Called by renderer to batch primitive data in source buffers
             * @param Final size of the graphics area
             * @param vertices Buffer to add vertices data
             * @param indices Buffer to add indices data
             * @param baseIndex Base index to add to this primitive indices
             * @param verticesAdded Number of vertices added by primitive
             * @param indicesAdded Number of indices added by primitive
             */
            virtual void packVertexData(const Size2i &area, DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                        uint32 &verticesAdded, uint32 &indicesAdded) = 0;
        };

        class GraphicsPrimitiveRect : public GraphicsPrimitive {
        public:
            static const uint32 POINTS = 4;
            static const uint32 INDICES = 6;

            Size2i size;

            void packVertexData(const Size2i &area, DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices,
                                uint32 baseIndex,
                                uint32 &verticesAdded, uint32 &indicesAdded) override {
                // v0 ---- v1
                // |       |
                // |       |
                // v3 ---- v2

                // Invert Y axis
                Point2i p = Point2i(position.x(), area.height() - position.y());

                vertices.append(p);
                vertices.append(color);

                vertices.append(Point2i(p.x() + size.width(), p.y()));
                vertices.append(color);

                vertices.append(Point2i(p.x() + size.width(), p.y() - size.height()));
                vertices.append(color);

                vertices.append(Point2i(p.x(), p.y() - size.height()));
                vertices.append(color);

                verticesAdded = POINTS;

                uint32 data[] = { baseIndex + 0, baseIndex + 3, baseIndex + 2, baseIndex + 2, baseIndex + 1, baseIndex + 0 };
                indices.append((const uint8*) data, sizeof(data));

                indicesAdded = INDICES;
            }
        };

    }
}

#endif //BERSERK_GRAPHICSITEMS_H
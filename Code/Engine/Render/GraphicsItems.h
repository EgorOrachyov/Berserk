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
            ~GraphicsPrimitive() override = default;

            /**
             * Called by renderer to batch primitive data in source buffers
             * @param Final size of the graphics area
             * @param vertices Buffer to add vertices data
             * @param indices Buffer to add indices data
             * @param baseIndex Base index to add to this primitive indices
             * @param verticesAdded Number of vertices added by primitive
             * @param indicesAdded Number of indices added by primitive
             */
            virtual void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                        uint32 &verticesAdded,
                                        uint32 &indicesAdded) = 0;
        };

        class GraphicsFilledRect : public GraphicsPrimitive {
        public:
            ~GraphicsFilledRect() override = default;

            static const uint32 POINTS = 4;
            static const uint32 INDICES = 6;

            Size2i size;

            void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                uint32 &verticesAdded,
                                uint32 &indicesAdded) override {
                // v0 ---- v1
                // |       |
                // |       |
                // v3 ---- v2

                vertices.append(position);
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width(), position.y()));
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width(), position.y() + size.height()));
                vertices.append(color);

                vertices.append(Point2i(position.x(), position.y() + size.height()));
                vertices.append(color);

                verticesAdded = POINTS;

                uint32 data[] = { baseIndex + 0, baseIndex + 3, baseIndex + 2, baseIndex + 2, baseIndex + 1, baseIndex + 0 };
                indices.append((const uint8*) data, sizeof(data));

                indicesAdded = INDICES;
            }
        };

        class GraphicsFilledEllipse : public GraphicsPrimitive {
        public:
            ~GraphicsFilledEllipse() override = default;

            static const uint32 MIN_SECTIONS = 4;

            Size2i radius;
            uint32 sections;

            void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                uint32 &verticesAdded, uint32 &indicesAdded) override {
                auto center = position + radius;
                auto pointsCount = sections + 1;
                auto indicesCount = sections * 3;
                auto centerIndex = sections;

                float rX = (float) radius[0];
                float rY = (float) radius[1];
                float a = 0.0f;
                float da = 2.0f * Math::PIf / (float) sections;

                for (uint32 i = 0; i < sections; i++) {
                    float x = rX * Math::cos(a);
                    float y = rY * Math::sin(a);

                    a += da;

                    auto p = Point2i(center[0] + (int32)x, center[1] + (int32)y);
                    auto i1 = i;
                    auto i2 = centerIndex;
                    auto i3 = (i + 1) % sections;

                    vertices.append(p);
                    vertices.append(color);
                    indices.append(baseIndex + i1);
                    indices.append(baseIndex + i2);
                    indices.append(baseIndex + i3);
                }

                vertices.append(center);
                vertices.append(color);

                verticesAdded = pointsCount;
                indicesAdded = indicesCount;
            }
        };

        class GraphicsLine : public GraphicsPrimitive {
        public:
            ~GraphicsLine() override = default;

            static const uint32 POINTS = 4;
            static const uint32 INDICES = 6;

            Point2i end;
            uint32 width;

            void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                uint32 &verticesAdded, uint32 &indicesAdded) override {
                auto p1 = position;
                auto p2 = end;
                auto d = p2 - p1;
                auto t = Point2i(-d[1], d[0]);

                int32 half1;
                int32 half2;

                if (width > 1) {
                    if (width % 2 == 0) {
                        half1 = width / 2;
                        half2 = width / 2 - 1;
                    }
                    else {
                        half1 = width / 2;
                        half2 = width / 2;
                    }
                }
                else {
                    half1 = 1;
                    half2 = 0;
                }

                auto a = Vec2f((float)t[0],(float)t[1]).normalized();
                float ax = a[0];
                float ay = a[1];
                float dup = (float) -half1;
                float ddown = (float) half2;

                // v0 ----- v1
                // v3 ----- v2

                vertices.append(Point2i(ax * dup + position[0], ay * dup + position[1]));
                vertices.append(color);

                vertices.append(Point2i(ax * dup + end[0], ay * dup + end[1]));
                vertices.append(color);

                vertices.append(Point2i(ax * ddown + end[0], ay * ddown + end[1]));
                vertices.append(color);

                vertices.append(Point2i(ax * ddown + position[0], ay * ddown + position[1]));
                vertices.append(color);

                indices.append(baseIndex + 0);
                indices.append(baseIndex + 3);
                indices.append(baseIndex + 2);
                indices.append(baseIndex + 2);
                indices.append(baseIndex + 1);
                indices.append(baseIndex + 0);

                verticesAdded = POINTS;
                indicesAdded = INDICES;
            }
        };

        class GraphicsRect : public GraphicsPrimitive {
        public:
            ~GraphicsRect() override = default;

            static const uint32 POINTS = 8;
            static const uint32 INDICES = 8 * 3;

            Size2i size;
            uint32 border;

            void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                uint32 &verticesAdded, uint32 &indicesAdded) override {
                // v0 ----------- v1
                // | v4 ------- v5|
                // | |          | |
                // | |          | |
                // | v7 ------- v6|
                // v3 ----------- v2

                vertices.append(position);
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width(), position.y()));
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width(), position.y() + size.height()));
                vertices.append(color);

                vertices.append(Point2i(position.x(), position.y() + size.height()));
                vertices.append(color);

                vertices.append(Point2i(position.x() + border, position.y() + border));
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width() - border, position.y() + border));
                vertices.append(color);

                vertices.append(Point2i(position.x() + size.width() - border, position.y() + size.height() - border));
                vertices.append(color);

                vertices.append(Point2i(position.x() + border, position.y() + size.height() - border));
                vertices.append(color);

                verticesAdded = POINTS;

                uint32 data[] = {
                        baseIndex + 0, baseIndex + 4, baseIndex + 1,
                        baseIndex + 4, baseIndex + 5, baseIndex + 1,
                        baseIndex + 1, baseIndex + 5, baseIndex + 2,
                        baseIndex + 5, baseIndex + 6, baseIndex + 2,
                        baseIndex + 7, baseIndex + 3, baseIndex + 2,
                        baseIndex + 7, baseIndex + 2, baseIndex + 6,
                        baseIndex + 0, baseIndex + 3, baseIndex + 7,
                        baseIndex + 0, baseIndex + 7, baseIndex + 4
                };

                indices.append((const uint8*) data, sizeof(data));

                indicesAdded = INDICES;
            }
        };


    }
}

#endif //BERSERK_GRAPHICSITEMS_H
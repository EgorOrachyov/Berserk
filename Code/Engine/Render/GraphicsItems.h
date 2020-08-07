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
#include <RenderResources/GpuFont.h>
#include <RenderResources/Texture2D.h>
#include <RenderResources/DynamicIndexBuffer.h>
#include <RenderResources/DynamicVertexBuffer.h>
#include <String/CString.h>
#include <String/WString.h>

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

            /** Utility to pack items per vertex data */
            struct Pack {
                int32 pos[3];
                float color[4];

                Pack(const Point2i& p, int32 z, const Color4f& c) {
                    pos[0] = p[0];
                    pos[1] = p[1];
                    pos[2] = z;
                    color[0] = c[0];
                    color[1] = c[1];
                    color[2] = c[2];
                    color[3] = c[3];
                }
            };
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

                vertices.append(Pack(position,zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width(), position.y()),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width(), position.y() + size.height()),zOrder,color));
                vertices.append(Pack(Point2i(position.x(), position.y() + size.height()),zOrder,color));

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

                    vertices.append(Pack(p,zOrder,color));
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

                vertices.append(Pack(Point2i(ax * dup + position[0], ay * dup + position[1]),zOrder,color));
                vertices.append(Pack(Point2i(ax * dup + end[0], ay * dup + end[1]),zOrder,color));
                vertices.append(Pack(Point2i(ax * ddown + end[0], ay * ddown + end[1]),zOrder,color));
                vertices.append(Pack(Point2i(ax * ddown + position[0], ay * ddown + position[1]),zOrder,color));

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

                vertices.append(Pack(position,zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width(), position.y()),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width(), position.y() + size.height()),zOrder,color));
                vertices.append(Pack(Point2i(position.x(), position.y() + size.height()),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + border, position.y() + border),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width() - border, position.y() + border),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + size.width() - border, position.y() + size.height() - border),zOrder,color));
                vertices.append(Pack(Point2i(position.x() + border, position.y() + size.height() - border),zOrder,color));

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

        class GraphicsEllipse : public GraphicsPrimitive {
        public:
            ~GraphicsEllipse() override = default;

            static const uint32 MIN_SECTIONS = 4;

            Size2i radius;
            uint32 sections;
            uint32 border;

            void packVertexData(DynamicVertexBuffer &vertices, DynamicIndexBuffer &indices, uint32 baseIndex,
                                uint32 &verticesAdded, uint32 &indicesAdded) override {
                auto center = position + radius;
                auto pointsCount = 2 * sections;
                auto indicesCount = sections * 3 * 2;

                float RX = (float) radius[0];
                float RY = (float) radius[1];
                float rX = (float) (radius[0] - (int32) border);
                float rY = (float) (radius[1] - (int32) border);
                float a = 0.0f;
                float da = 2.0f * Math::PIf / (float) sections;

                for (uint32 i = 0; i < sections; i++) {
                    float c = Math::cos(a);
                    float s = Math::sin(a);

                    float x = RX * c;
                    float y = RY * s;
                    float xIn = rX * c;
                    float yIn = rY * s;

                    a += da;

                    auto p1 = Point2i(center[0] + (int32)x, center[1] + (int32)y);
                    auto p2 = Point2i(center[0] + (int32)xIn, center[1] + (int32)yIn);

                    uint32 i1 = (i * 2 + 0) % pointsCount;
                    uint32 i2 = (i * 2 + 1) % pointsCount;
                    uint32 i3 = (i * 2 + 3) % pointsCount;
                    uint32 i4 = (i * 2 + 0) % pointsCount;
                    uint32 i5 = (i * 2 + 3) % pointsCount;
                    uint32 i6 = (i * 2 + 2) % pointsCount;

                    vertices.append(Pack(p1,zOrder,color));
                    vertices.append(Pack(p2,zOrder,color));

                    indices.append(baseIndex + i1);
                    indices.append(baseIndex + i2);
                    indices.append(baseIndex + i3);
                    indices.append(baseIndex + i4);
                    indices.append(baseIndex + i5);
                    indices.append(baseIndex + i6);
                }

                verticesAdded = pointsCount;
                indicesAdded = indicesCount;
            }
        };

        /** Represents single request to draw graphics text */
        class GraphicsText : public GraphicsItem {
        public:
            ~GraphicsText() override = default;

            static const uint32 VERTICES_PER_GLYPH = 6;

            WString text;
            uint32 height = 0;
            TPtrShared<GpuFont> font;
            TPtrShared<RHIUniformSet> uniformBinding;

            void packVertexData(DynamicVertexBuffer &vertices, uint32 &verticesAdded) {
                float scale = (height > 0? (float) height / (float) font->getFontSize().height() : 1.0f);

                FontGlyph null;
                font->getGlyph(Font::NULL_GLYPH, null);

                verticesAdded = 0;
                int32 advanceX = 0;

                for (uint32 i = 0; i < text.length(); i++) {
                    wchar c = text.data()[i];

                    FontGlyph glyph;
                    auto found = font->getGlyph(c, glyph);
                    if (!found) glyph = null;

                    int32 x0 = advanceX + position.x() +  (int32)(scale * (float) glyph.bearing.x());
                    int32 y0 = position.y() -  (int32)(scale * (float) glyph.bearing.y());
                    int32 x1 = x0 + (int32)(scale * (float)glyph.size.x());
                    int32 y1 = y0 + (int32)(scale * (float)glyph.size.y());

                    // p0 ---- p1
                    // |       |
                    // |       |
                    // p3 ---- p2

                    Point2i p0(x0,y0);
                    Point2i p1(x1,y0);
                    Point2i p2(x1,y1);
                    Point2i p3(x0,y1);

                    int32 u0 = 0;
                    int32 v0 = 1;
                    int32 u1 = 2;
                    int32 v1 = 3;

                    Vec2f t0(glyph.uvs[u0],glyph.uvs[v0]);
                    Vec2f t1(glyph.uvs[u1],glyph.uvs[v0]);
                    Vec2f t2(glyph.uvs[u1],glyph.uvs[v1]);
                    Vec2f t3(glyph.uvs[u0],glyph.uvs[v1]);

                    struct Pack {
                        int32 p[3];
                        float c[4];
                        float t[2];

                        Pack(const Point2i& point, int32 z, const Color4f& color, const Vec2f& tex) {
                            p[0] = point[0];
                            p[1] = point[1];
                            p[2] = z;
                            c[0] = color[0];
                            c[1] = color[1];
                            c[2] = color[2];
                            c[3] = color[3];
                            t[0] = tex[0];
                            t[1] = tex[1];
                        }
                    };

                    Pack pck0(p0,zOrder,color,t0);
                    Pack pck1(p1,zOrder,color,t1);
                    Pack pck2(p2,zOrder,color,t2);
                    Pack pck3(p3,zOrder,color,t3);

                    vertices.append(pck0);
                    vertices.append(pck3);
                    vertices.append(pck2);

                    vertices.append(pck2);
                    vertices.append(pck1);
                    vertices.append(pck0);

                    verticesAdded += VERTICES_PER_GLYPH;
                    advanceX += (int32)(scale * (float)glyph.advance.x());
                }
            }
        };

    }
}

#endif //BERSERK_GRAPHICSITEMS_H
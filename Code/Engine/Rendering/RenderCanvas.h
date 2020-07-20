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
#include <Math/Mat4x4f.h>
#include <Rendering/VertexPolicy.h>
#include <Rendering/RenderTarget.h>
#include <Rendering/Resources/Shader.h>
#include <Rendering/Resources/UniformBuffer.h>
#include <Rendering/Resources/GraphicsPipeline.h>
#include <Rendering/Resources/VertexBufferCached.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Rendering canvas
         *
         * Rendering surface for processing 2D primitives such as text,
         * lines, points, circles and simple 3D primitives such as boxes, spheres.
         */
        class RenderCanvas {
        public:

            RenderCanvas(TPtrShared<RenderTarget> target);
            ~RenderCanvas() = default;

            // todo
            void drawLine(const Vec2f &from, const Vec2f &to, const Color4f &color) {}
            void drawLine(const Vec3f &from, const Vec3f &to, const Color4f &color) {}

            /**
             * Draw sphere in 3D space with specified position and radius.
             * @param pos World space position
             * @param radius Sphere radius in world units
             * @param color Sphere color
             */
            void drawSphere(const Vec3f &pos, float radius, const Color4f &color);

            /**
             * Capture draw commands for rendering in specified list
             * @param drawList List to write draw request
             */
            void captureDrawCommands(RHIDrawList &drawList);

            /**
             * Clear canvas (removes all draw request).
             * @note Typical draw usage is to perform clear and draw request collect on each frame.
             */
            void clearCanvas();

            void setPerspective(const Mat4x4f& persp) { mPerspective = persp; };
            void setOrthographic(const Mat4x4f& ortho) { mOrthographic = ortho; };
            void setView(const Mat4x4f& view) { mView = view; };

        protected:

            void createCommonData();
            void createSphereData();

            Mat4x4f mPerspective;
            Mat4x4f mOrthographic;
            Mat4x4f mView;

            /** Target used for drawing this canvas data */
            TPtrShared<RenderTarget> mDrawTarget;

            struct SphereDraw {
                float radius;
                Vec3f pos;
                Color4f color;
            };

            TPtrShared<Shader> mSphere3dShader;
            TPtrShared<RHIVertexDeclaration> mSphere3dDeclaration;
            TPtrUnique<GraphicsPipeline> mSphere3dPipeline;
            TPtrUnique<UniformBuffer> mCommonData;
            TPtrShared<RHIUniformSet> mCommonSet;

            uint32 mMaxSpheresInGPU = 100;
            uint32 mSphere3dIndicesCount;
            TArray<SphereDraw> mSphere3dRequests;
            TArray<VertexBufferCached> mSphere3dBuffers;
            TArray<TPtrShared<RHIArrayObject>> mSphere3dObjects;

        };

    }
}

#endif //BERSERK_RENDERCANVAS_H
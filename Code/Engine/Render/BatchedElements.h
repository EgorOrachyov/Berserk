/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BATCHEDELEMENTS_H
#define BERSERK_BATCHEDELEMENTS_H

#include <Math/Math.h>
#include <Math/Vec2f.h>
#include <Math/Vec3f.h>
#include <Math/Mat4x4f.h>
#include <Math/Color4.h>
#include <Containers/TArray.h>

namespace Berserk {
    namespace Render {

        struct BatchedLine {
            Vec3f begin;
            Vec3f end;
            Color4f color;
        };

        struct BatchedPoint {
            Vec3f position;
            float size;
            Color4f color;
        };

        struct BatchedBox {
            Vec3f position;
            Vec3f size;
            Color4f color;
            Mat4x4f rotation;
            bool wire;
        };

        struct BatchedSphere {
            Vec3f position;
            float radius;
            Color4f color;
            bool wire;
        };

        /**
         * @brief Batched elements
         *
         * Container for basic rendering primitives, grouped by type and later rendered
         * in the view by groups with instancing acceleration, instead of per primitive submit to GPU.
         */
        class BatchedElements {
        public:

            /** Add line to the batch */
            void addLine(const Vec3f& begin, const Vec3f& end, const Color4f& color);

            /** Add point to the batch */
            void addPoint(const Vec3f& position, float size, const Color4f& color);

            /** Add filled box to the batch */
            void addBox(const Vec3f& position, const Vec3f& size, const Color4f& color, const Mat4x4f& rotation);

            /** Add filled sphere to the batch */
            void addSphere(const Vec3f& position, float radius, const Color4f& color);

            /** Add wire frame box to the batch */
            void addWireBox(const Vec3f& position, const Vec3f& size, const Color4f& color, const Mat4x4f& rotation);

            /** Add wire frame sphere to the batch */
            void addWireSphere(const Vec3f& position, float radius, const Color4f& color);

            /** Clear batch. Remove all batched primitives */
            void clear();

            /** @return True if has no elements */
            bool isEmpty() const;

            /** @return Batched lines data */
            const TArray<BatchedLine> &getLines() const { return mLines; }

            /** @return Batched points data */
            const TArray<BatchedPoint> &getPoints() const { return mPoints; }

            /** @return Batched boxes data */
            const TArray<BatchedBox> &getBoxes() const { return mBoxes; }

            /** @return Batched spheres data */
            const TArray<BatchedSphere> &getSpheres() const { return  mSpheres; }

        private:
            TArray<BatchedLine> mLines;
            TArray<BatchedPoint> mPoints;
            TArray<BatchedBox> mBoxes;
            TArray<BatchedSphere> mSpheres;
        };


    }
}

#endif //BERSERK_BATCHEDELEMENTS_H
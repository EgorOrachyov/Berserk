/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <BatchedElements.h>

namespace Berserk {
    namespace Render {

        void BatchedElements::addLine(const Vec3f& begin, const Vec3f& end, const LinearColor& color) {
            auto& line = mLines.emplace();
            line.begin = begin;
            line.end = end;
            line.color = color;
        }

        void BatchedElements::addPoint(const Vec3f& position, float size, const LinearColor& color) {
            auto& point = mPoints.emplace();
            point.position = position;
            point.size = size;
            point.color = color;
        }

        void BatchedElements::addBox(const Vec3f& position, const Vec3f& size, const LinearColor& color, const Mat4x4f &rotation) {
            auto& box = mBoxes.emplace();
            box.position = position;
            box.size = size;
            box.color = color;
            box.rotation = rotation;
            box.wire = false;
        }

        void BatchedElements::addSphere(const Vec3f& position, float radius, const LinearColor& color) {
            auto& sphere = mSpheres.emplace();
            sphere.position = position;
            sphere.radius = radius;
            sphere.color = color;
            sphere.wire = false;
        }

        void BatchedElements::addWireBox(const Vec3f& position, const Vec3f& size, const LinearColor& color, const Mat4x4f &rotation) {
            auto& box = mBoxes.emplace();
            box.position = position;
            box.size = size;
            box.color = color;
            box.rotation = rotation;
            box.wire = true;
        }

        void BatchedElements::addWireSphere(const Vec3f& position, float radius, const LinearColor& color) {
            auto& sphere = mSpheres.emplace();
            sphere.position = position;
            sphere.radius = radius;
            sphere.color = color;
            sphere.wire = true;
        }

        void BatchedElements::clear() {
            mLines.clear();
            mPoints.clear();
            mBoxes.clear();
            mSpheres.clear();
        }

    }
}
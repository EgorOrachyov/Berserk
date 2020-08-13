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

        void BatchedElements::addLine(const Vec3f& begin, const Vec3f& end, const Color4f& color) {
            auto& line = mLines.emplace();
            line.begin = begin;
            line.end = end;
            line.color = color;
        }

        void BatchedElements::addPoint(const Vec3f& position, float size, const Color4f& color) {
            auto& point = mPoints.emplace();
            point.position = position;
            point.size = size;
            point.color = color;
        }

        void BatchedElements::addBox(const Vec3f& position, const Vec3f& size, const Color4f& color, const Quatf &rotation) {
            auto& box = mBoxes.emplace();
            box.position = position;
            box.size = size;
            box.color = color;
            box.rotation = rotation;
            box.wire = false;
        }

        void BatchedElements::addAabb(const Aabbf &box, const Color4f &color) {
            auto& b = mBoxes.emplace();
            b.position = box.getCenter();
            b.size = box.getExtent() * 2.0f;
            b.color = color;
            b.rotation = Quatf();
            b.wire = false;
        }

        void BatchedElements::addCylinder(const Vec3f &position, const Vec2f &radius, float height, const Color4f &color, const Quatf &rotation) {
            auto& cylinder = mCylinders.emplace();
            cylinder.position = position;
            cylinder.sizeRxRyH = Vec3f(radius[0], radius[1], height);
            cylinder.color = color;
            cylinder.rotation = rotation;
            cylinder.wire = false;
        }

        void BatchedElements::addSphere(const Vec3f& position, float radius, const Color4f& color) {
            auto& sphere = mSpheres.emplace();
            sphere.position = position;
            sphere.radius = radius;
            sphere.color = color;
            sphere.wire = false;
        }

        void BatchedElements::addWireBox(const Vec3f& position, const Vec3f& size, const Color4f& color, const Quatf &rotation) {
            auto& box = mBoxes.emplace();
            box.position = position;
            box.size = size;
            box.color = color;
            box.rotation = rotation;
            box.wire = true;
        }
        
        void BatchedElements::addWireAabb(const Aabbf &box, const Color4f &color) {
            auto& b = mBoxes.emplace();
            b.position = box.getCenter();
            b.size = box.getExtent() * 2.0f;
            b.color = color;
            b.rotation = Quatf();
            b.wire = true;
        }

        void BatchedElements::addWireCylinder(const Vec3f &position, const Vec2f &radius, float height, const Color4f &color, const Quatf &rotation) {
            auto& cylinder = mCylinders.emplace();
            cylinder.position = position;
            cylinder.sizeRxRyH = Vec3f(radius[0], radius[1], height);
            cylinder.color = color;
            cylinder.rotation = rotation;
            cylinder.wire = true;
        }

        void BatchedElements::addWireSphere(const Vec3f& position, float radius, const Color4f& color) {
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
            mCylinders.clear();
        }

        bool BatchedElements::isEmpty() const {
            return mLines.isEmpty() && mPoints.isEmpty() && mSpheres.isEmpty() && mBoxes.isEmpty() && mCylinders.isEmpty();
        }

    }
}
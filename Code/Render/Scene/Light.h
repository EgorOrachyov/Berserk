/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_LIGHT_H
#define BERSERK_LIGHT_H

#include <Typedefs.h>
#include <Math/Vec3f.h>
#include <Math/Transformf.h>

namespace Berserk {
    namespace Render {

        /** Build in engine lights types */
        enum class ELightType : uint8 {
            Directional = 0,
            Spot = 1,
            Point = 2,
            Unknown
        };

        /**
         * @brief Light source
         *
         * Represent 3d-space light source, which could be added to the scene.
         * Number of the light sources is limited at compile time.
         *
         * By default the light source has direction as Z+ axis and up as Y+ axis.
         * The direction and up could be changed by transform apply.
         */
        class Light {
        public:
            static const uint32 TYPE_DIRECTIONAL = (uint32)ELightType::Directional;
            static const uint32 TYPE_SPOT = (uint32)ELightType::Spot;
            static const uint32 TYPE_POINT = (uint32)ELightType::Point;

            /** Creates directional light */
            Light(const Vec3f& energy);

            /** Creates spot light */
            Light(const Vec3f& energy, float spotInner, float spotOuter, float distance);

            /** Creates point light */
            Light(const Vec3f& energy, float distance);

            /** @return Light source type */
            ELightType getType() const { return mType; }

            /** Set world space transform */
            void setTransform(const Transformf& t);

            /** @return World-space light source transform */
            const Transformf& getTransform() const { return mTransform; }

        private:

            /** Energy per color channel */
            Vec3f mEnergy;

            /** World space position */
            Vec3f mWorldPosition;

            /** Light source type */
            ELightType mType = ELightType::Unknown;

            /** Action distance (for point and spot) */
            float mDistance;

            /** Inner spot angle in Rad (for smooth borders) */
            float mSpotAngleInner;

            /** Outer spot angle in Rad (for smooth borders) */
            float mSpotAngleOuter;

            /** World space transformation */
            Transformf mTransform;

        };

    }
}

#endif //BERSERK_LIGHT_H
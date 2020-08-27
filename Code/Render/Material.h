/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include <TEnumMask.h>
#include <GpuMeshAttribute.h>
#include <Containers/TArrayStatic.h>
#include <RenderResources/Texture.h>
#include <RenderResources/UniformBuffer.h>
#include <RenderResources/RenderResource.h>
#include <Shader.h>

namespace Berserk {
    namespace Render {

        /** Material physical type, critical for performance */
        enum class EMaterialType : uint8 {
            Opaque = 0,
            Transparent = 1
        };

        /** Textures, provided by material */
        enum class EMaterialTexture : uint8 {
            Albedo = 0,
            Specular = 1,
            Metallic = 2,
            Roughness = 3,
            Normal = 4,
            Displacement = 5,
            AmbientOcclusion = 6,
            Emission = 7,
            Max = 8
        };

        /** Material flags, which allows to tweak shading process */
        enum class EMaterialFlag : uint8 {
            DoNotReceivesShadows = 0,
        };

        /** Defines shading algorithm, used for the material */
        enum class EMaterialShading : uint8 {
            BlinnPhong = 0,
            PBR = 1,
            Custom = 2,
            Max = 3
        };

        /** Define presented textures as features */
        using EMaterialFeature = EMaterialTexture;

        /** Defines texture features of the material */
        using MaterialFeatures = TEnumMask<EMaterialFeature>;

        /** Defines material flags, to tweak rendering  process */
        using MaterialFlags = TEnumMask<EMaterialFlag>;

        /**
         * @brief Material class
         *
         * Generic material class, which can be applied to the rendered geometry.
         * Material represents visual properties of the rendered geometry: meshes,
         * animated or skinned objects, terrain, vegetation, etc.
         *
         * Material at its base allows to tweak predefined accessible params, such
         * as textures, colors, flags, as well as rendering settings, such as
         * shadowing type, emission, normal, displacement mapping and etc.
         *
         * By default, the engine provides default shaders set, which are capable of
         * rendering on GPU the basic material. By user allowed to provides his
         * shader code, which will be gently inserted into engine shaders rendering environment.
         *
         * @note Material is render thread (system) object only
         * @note Share the same material instance among objects in order to increase performance
         * @note Use material clone API to create new material instance from chosen one
         */
        class Material {
        public:

            static const uint32 MAX_TEXTURES = (uint32)EMaterialTexture::Max;
            static const float DEFAULT_INVERSE_GAMMA;

            Material();
            ~Material() = default;

            void setName(CString name);
            void setAlbedo(const Color4f& albedo);
            void setEmission(const Color4f& emission);
            void setAlpha(float alpha);
            void setSpecular(float specular);
            void setMetallic(float metallic);
            void setRoughness(float roughness);
            void setAmbientScale(float scale);
            void setInverseGamma(float inverseGamma);
            void setType(EMaterialType materialType);
            void setShading(EMaterialShading shading);
            void setFlags(const MaterialFlags& flags);
            void setTexture(TPtrShared<Texture> texture, EMaterialTexture type);

            const CString& getName() const { return mName; }
            const Color4f& getAlbedo() const { return mAlbedo; }
            const Color4f& getEmission() const { return mEmission; }
            float getAlpha() const { return mAlpha; }
            float getSpecular() const { return mSpecular; }
            float getMetallic() const { return mMetallic; }
            float getRoughness() const { return mRoughness; }
            float getAmbientScale() const { return mAmbientScale; }
            float getInverseGamma() const { return mInverseGamma; }
            EMaterialType getType() const { return mType; }
            EMaterialShading getShading() const { return mShading; }
            MaterialFlags getFlags() const { return mFlags; }
            MaterialFeatures getFeatures() const { return mFeatures; }
            TPtrShared<Texture> getTexture(EMaterialTexture type) const;
            const TArrayStatic<TPtrShared<Texture>> &getTextures() const { return mTextures; }

        protected:

            CString mName;
            Color4f mAlbedo = Color4f(1.0f);
            Color4f mEmission = Color4f(1.0f);
            float mAlpha = 1.0f;
            float mSpecular = 1.0f;
            float mMetallic = 1.0f;
            float mRoughness = 1.0f;
            float mAmbientScale = 1.0f;
            float mInverseGamma = DEFAULT_INVERSE_GAMMA;
            EMaterialType mType = EMaterialType::Opaque;
            EMaterialShading mShading = EMaterialShading::BlinnPhong;
            MaterialFlags mFlags;
            MaterialFeatures mFeatures;
            TArrayStatic<TPtrShared<Texture>,MAX_TEXTURES> mTextures;
        };

    }
}

#endif //BERSERK_MATERIAL_H
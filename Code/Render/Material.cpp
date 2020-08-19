/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Material.h>
#include <TextureManager.h>

namespace Berserk {
    namespace Render {

        const float Material::DEFAULT_INVERSE_GAMMA = 1.0f / 2.4f;

        Material::Material() {
            // Ensure, that has all units allocated
            // null is ok, since null will be ignored on uniform set creation
            mTextures.resize(MAX_TEXTURES, nullptr);
        }

        void Material::setName(CString name) {
            mName = std::move(name);
        }

        void Material::setAlbedo(const Color4f &albedo) {
            mAlbedo = Color4f::max(albedo, Vec4f());
        }

        void Material::setEmission(const Color4f &emission) {
            mEmission = Color4f::max(emission, Vec4f());
        }

        void Material::setAlpha(float alpha) {
            mAlpha = alpha;
        }

        void Material::setSpecular(float specular) {
            mSpecular = Math::clamp(specular, 0.0f, 1.0f);
        }

        void Material::setMetallic(float metallic) {
            mMetallic = Math::clamp(metallic, 0.0f, 1.0f);
        }

        void Material::setRoughness(float roughness) {
            mRoughness = Math::clamp(roughness, 0.0f, 1.0f);
        }

        void Material::setAmbientScale(float scale) {
            mAmbientScale = Math::clamp(scale, 0.0f, 1.0f);
        }

        void Material::setInverseGamma(float inverseGamma) {
            mInverseGamma = inverseGamma > 0.0f? inverseGamma : DEFAULT_INVERSE_GAMMA;
        }

        void Material::setType(EMaterialType materialType) {
            mType = materialType;
        }

        void Material::setShading(EMaterialShading shading) {
            mShading = shading;
        }

        void Material::setFlags(const MaterialFlags &flags) {
            mFlags = flags;
        }

        void Material::setTexture(TPtrShared<Texture> texture, EMaterialTexture type) {
            BERSERK_COND_ERROR_RET(texture.isNotNull(), "Passed null texture");
            BERSERK_COND_ERROR_RET(texture->isInitializedRHI(), "Passed not initialized texture");

            auto index = (uint32)type;

            if (index < mTextures.size()) {
                mTextures[index] = std::move(texture);
                mFeatures.setFlag(type, true);
            }
        }

        void Material::notifyListeners() {
            for (auto listener: mListeners) {
                listener->notifyOnChanged();
            }
        }

        void Material::subscribe(MaterialListener &listener) {
            auto ptr = &listener;

            BERSERK_COND_ERROR_RET(!mListeners.contains(ptr), "Listener already subscribed");
            mListeners.add(ptr);
        }

        void Material::unsubscribe(MaterialListener &listener) {
            auto ptr = &listener;

            BERSERK_COND_ERROR_RET(mListeners.contains(ptr), "Listener was not subscribed");
            mListeners.removeElementUnordered(ptr);
        }
        
        TPtrShared<Texture> Material::getTexture(EMaterialTexture type) const {
            auto index = (uint32)type;

            if (index < mTextures.size()) {
                return mTextures[index];
            }

            return nullptr;
        }

    }
}
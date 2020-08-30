/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <MaterialBuilder.h>

namespace Berserk {
    namespace Render {

        MaterialBuilder& MaterialBuilder::setName(CString name) {
            mName = std::move(name);
            return *this;
        }

        MaterialBuilder& MaterialBuilder::setShading(EMaterialShading shading) {
            mShading = shading;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::setBlending(EMaterialBlending materialBlend) {
            mBlendingType = materialBlend;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::addParam(CString name, EShaderData type) {
            BERSERK_COND_ERROR_RET_VALUE(*this, !mParams.contains(name), "Param (%s) already present in the material: %s", name.data(), mName.data());

            MaterialParam param;
            param.name = name;
            param.dataType = type;
            param.isTexture = RHIDefinitionsUtil::isShaderDataTextureType(type);

            mParams.move(name, param);

            return *this;
        }

        MaterialBuilder& MaterialBuilder::setFlipUV(bool set) {
            mFlipUV = set;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::setWriteColor(bool set) {
            mWriteColor = set;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::setWriteDepth(bool set) {
            mWriteDepth = set;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::setDoubleSided(bool set) {
            mDoubleSided = set;
            return *this;
        }

        MaterialBuilder& MaterialBuilder::provideRequiredAttributes(TArray<EMeshAttribute> attributes) {
            mRequiredAttributes = std::move(attributes);
            return *this;
        }

        MaterialBuilder& MaterialBuilder::provideUserVertexCode(BinaryData vertex) {
            mUserVertexCode = std::move(vertex);
            return *this;
        }

        MaterialBuilder& MaterialBuilder::provideUserFragmentCode(BinaryData fragment) {
            mUserFragmentCode = std::move(fragment);
            return *this;
        }

        TPtrShared<Material> MaterialBuilder::buildShared() {
            return mCachedInstance.isNotNull() ? mCachedInstance : createInstanceAndCache();
        }

        TPtrShared<Material> MaterialBuilder::createInstanceAndCache() {
            mCachedInstance = TPtrShared<Material>::make(*this);
            return mCachedInstance;
        }
        
    }
}
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Material.h>
#include <MaterialBuilder.h>
#include <MaterialInstance.h>

namespace Berserk {
    namespace Render {

        Material::Material(MaterialBuilder &builder) {
            // Simple move from builder

            mName = std::move(builder.mName);
            mFlipUV = builder.mFlipUV;
            mWriteColor = builder.mWriteColor;
            mWriteDepth = builder.mWriteDepth;
            mDoubleSided = builder.mDoubleSided;
            mShading = builder.mShading;
            mBlendingType = builder.mBlendingType;
            mParams = std::move(builder.mParams);
            mRequiredAttributes = std::move(builder.mRequiredAttributes);
            mUserVertexCode = std::move(builder.mUserVertexCode);
            mUserFragmentCode = std::move(builder.mUserFragmentCode);
        }

        Material::~Material() {

        }

        TPtrShared<Shader> Material::getShader(const MaterialVariant &variant) {
            return nullptr;
        }

        TPtrShared<class MaterialInstance> Material::createInstance(CString name) {
            return nullptr;
        }


    }
}
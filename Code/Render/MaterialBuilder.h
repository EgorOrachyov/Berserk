/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALBUILDER_H
#define BERSERK_MATERIALBUILDER_H

#include <Material.h>

namespace Berserk {
    namespace Render {

        /** Utility builder for safe material construction */
        class MaterialBuilder {
        public:

            MaterialBuilder();
            ~MaterialBuilder() = default;

            MaterialBuilder& setName(CString name);

            MaterialBuilder& setShading(EMaterialShading shading);

            MaterialBuilder& setBlending(EMaterialBlending materialBlend);

            MaterialBuilder& addParam(CString name, EShaderData type);

            MaterialBuilder& setFlipUV(bool set);

            MaterialBuilder& setWriteColor(bool set);

            MaterialBuilder& setWriteDepth(bool set);

            MaterialBuilder& setDoubleSided(bool set);

            MaterialBuilder& provideUserVertexCode(BinaryData vertex);

            MaterialBuilder& provideUserFragmentCode(BinaryData fragment);

            TPtrShared<Material> buildShared();

        private:
            CString mName;
            bool mFlipUV = false;
            bool mWriteColor = false;
            bool mWriteDepth = false;
            bool mDoubleSided = false;
            TMap<CString,MaterialParam> mParams;
            TArray<EMeshAttribute> mRequiredAttributes;
            EMaterialShading mShading = EMaterialShading::BlinnPhong;
            EMaterialBlending mBlendingType = EMaterialBlending::Opaque;
            BinaryData mUserVertexCode;
            BinaryData mUserFragmentCode;
        };

    }
}

#endif //BERSERK_MATERIALBUILDER_H
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

            /** Set optional material name (must be unique if material will be cached somewhere) */
            MaterialBuilder& setName(CString name);

            /** Set vertices input domain type */
            MaterialBuilder& setDomain(EMaterialDomain domain);

            /** Generic shading model setup */
            MaterialBuilder& setShading(EMaterialShading shading);

            /** Blending mode */
            MaterialBuilder& setBlending(EMaterialBlending materialBlend);

            /** Adds user defined param */
            MaterialBuilder& addParam(CString name, EShaderData type);

            /** Set true to flip texture coords (y = 1.0 - y) */
            MaterialBuilder& setFlipUV(bool set);

            /** Set true to write result color in color buffer */
            MaterialBuilder& setWriteColor(bool set);

            /** Set true to write result depth value in depth buffer */
            MaterialBuilder& setWriteDepth(bool set);

            /** Set true to render object with this material for both sides */
            MaterialBuilder& setDoubleSided(bool set);

            /** Explicitly provides required vertex inputs */
            MaterialBuilder& provideRequiredAttributes(TArray<EMeshAttribute> attributes);

            /** Optional user vertex code */
            MaterialBuilder& provideUserVertexCode(BinaryData vertex);

            /** Optional user fragment code */
            MaterialBuilder& provideUserFragmentCode(BinaryData fragment);

            /** @return Built material object (null if failed to create) */
            TPtrShared<Material> buildShared();

        private:
            friend class Material;

            /** Internally creates instance */
            TPtrShared<Material> createInstanceAndCache();

            CString mName;
            bool mFlipUV = false;
            bool mWriteColor = false;
            bool mWriteDepth = false;
            bool mDoubleSided = false;
            TMap<CString,MaterialParam> mParams;
            TArray<EMeshAttribute> mRequiredAttributes;
            EMaterialDomain mDomain = EMaterialDomain::Model;
            EMaterialShading mShading = EMaterialShading::Unlit;
            EMaterialBlending mBlendingType = EMaterialBlending::Opaque;
            BinaryData mUserVertexCode;
            BinaryData mUserFragmentCode;
            TPtrShared<Material> mCachedInstance;

            bool mHasError = false;
            CString mErrorMessage;
        };

    }
}

#endif //BERSERK_MATERIALBUILDER_H
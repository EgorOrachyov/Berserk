/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIAL_H
#define BERSERK_MATERIAL_H

#include <Shader.h>
#include <TPtrShared.h>
#include <GpuMeshFormat.h>
#include <MaterialParam.h>
#include <MaterialVariant.h>
#include <MaterialDefinitions.h>
#include <Containers/TMap.h>
#include <Containers/TArrayStatic.h>
#include <UniformBlockLayout.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Material class
         *
         * Represents material type - named drawing capabilities, which must be defined in
         * compile time of the corresponding material shaders. Material behaves as a
         * material factory for material instances, which are actually assigned to the rendered geometry.
         *
         * Material defines shading algorithm, provides user code, declares user params and required
         * input vertex attributes.
         *
         * Material also caches and compiles shaders for different geometry permutations.
         * This shaders are compiled on demand and queried in time of the rendering
         * of the concrete geometry.
         *
         */
        class Material {
        public:
            /** Initializes material from builder (only) */
            Material(class MaterialBuilder& builder);
            ~Material();

            /** @return Optional name */
            const CString& getName() const { return mName; }

            /** @return Whether to flit textures uvs */
            bool getFlipUV() const { return mFlipUV; }

            /** @return True if write color value */
            bool getWriteColor() const { return mWriteColor; }

            /** @return True if must write depth on rendering */
            bool getWriteDepth() const { return mWriteDepth; }

            /** @return True if render for both sides */
            bool getDoubleSided() const { return mDoubleSided; }

            /** @return True if user provides custom vertex shader code */
            bool hasUserVertexCode() const { return mUserVertexCode.size() > 0; }

            /** @return True if user provides custom fragment shader code */
            bool hasUserFragmentCode() const { return mUserFragmentCode.size() > 0; }

            /** @return Input vertices domain type */
            EMaterialDomain getDomain() const { return mDomain; }

            /** @return Material blending type */
            EMaterialBlending getType() const { return mBlendingType; }

            /** @return Material shading type */
            EMaterialShading getShading() const { return mShading; }

            /** @return Exposed user params */
            const TMap<CString,MaterialParam> &getParams() const { return mParams; };

            /** @return Attributes explicitly required by user */
            const TArray<EMeshAttribute> & getRequiredAttributes() const { return mRequiredAttributes; };

            /** Layout of the data defined by user */
            const UniformBlockLayout& getUniformUserLayout() const { return mUniformUserLayout; }

            /** @return Shader for requested variant type */
            TPtrShared<Shader> getShader(const MaterialVariant& variant);

            /** @return Material instance of the material type */
            TPtrShared<class MaterialInstance> createInstance(CString name);

        protected:

            TArray<TPtrShared<Shader>> mCachedShaders;

            CString mName;
            bool mFlipUV;
            bool mWriteColor;
            bool mWriteDepth;
            bool mDoubleSided;
            EMaterialDomain mDomain = EMaterialDomain::Model;
            EMaterialShading mShading = EMaterialShading::Unlit;
            EMaterialBlending mBlendingType = EMaterialBlending::Opaque;
            TMap<CString,MaterialParam> mParams;
            TArray<EMeshAttribute> mRequiredAttributes;

            BinaryData mUserVertexCode;
            BinaryData mUserFragmentCode;

            TPtrShared<class MaterialInstance> mDefaultInstance;
            UniformBlockLayout mUniformUserLayout;

            RHIGraphicsPipelineState mPipelineBase;
        };

    }
}

#endif //BERSERK_MATERIAL_H
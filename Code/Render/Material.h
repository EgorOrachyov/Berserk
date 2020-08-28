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
#include <MaterialDefinitions.h>
#include <Containers/TMap.h>
#include <Containers/TArrayStatic.h>

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

            /** Optional name */
            const CString& getName() const { return mName; }

            /** @return Material blending type */
            EMaterialBlending getType() const { return mType; }

            /** @return Material shading type */
            EMaterialShading getShading() const { return mShading; }

            /** @return Exposed user params */
            const TMap<CString,MaterialParam> & getParams() const { return mParams; };

            /** @return Shader for requested variant type */
            TPtrShared<TPtrShared<Shader>> getShader();

            /** @return Material instance of the material type */
            TPtrShared<class MaterialInstance> createInstance();

        protected:

            /** Optional name */
            CString mName;

            /** Whether to flit textures uvs */
            bool mFlipUV;

            /** True if write color value */
            bool mWriteColor;

            /** True if must write depth on rendering */
            bool mWriteDepth;

            /** True if render for both sides */
            bool mDoubleSided;

            /** Shading model */
            EMaterialShading mShading = EMaterialShading::Unlit;

            /** Blending type */
            EMaterialBlending mType = EMaterialBlending::Opaque;

            /** Material params */
            TMap<CString,MaterialParam> mParams;

            /** Attributes explicitly required by user */
            TArray<EMeshAttribute> mRequiredAttributes;

            /** Compiled shaders, associated with this material */
            TArray<TPtrShared<Shader>> mCachedShaders;

            /** User provided vertex shader code */
            BinaryData mUserVertexCode;

            /** User provided fragment shader code */
            BinaryData mUserFragmentCode;
        };

    }
}

#endif //BERSERK_MATERIAL_H
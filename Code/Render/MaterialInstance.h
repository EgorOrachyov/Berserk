/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALINSTANCE_H
#define BERSERK_MATERIALINSTANCE_H

#include <Material.h>
#include <RHI/RHIUniformSet.h>
#include <RenderResources/UniformBuffer.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Material Instance
         *
         * Represents actual, optionally named, material instance, which could be actually
         * attached to the geometry for the rendering.
         *
         * Material instance represents a container for user provided variables,
         * defined in the material type.
         */
        class MaterialInstance {
        public:

            /** Typedefs for params, exposed by material */
            using Bool = bool;
            using Vec2 = TVecN<float,2>;
            using Vec3 = TVecN<float,3>;
            using Vec4 = TVecN<float,4>;
            using Vec2i = TVecN<int32,2>;
            using Vec3i = TVecN<int32,3>;
            using Vec4i = TVecN<int32,4>;
            using Mat2 = TMatMxN<float,2,2>;
            using Mat3 = TMatMxN<float,3,3>;
            using Mat4 = TMatMxN<float,4,4>;

            /** Creates material instance for specified material */
            MaterialInstance(CString name, Material& material);

            /** Set bool value with specified name */
            void setBool(const CString& name, bool t, uint32 index = 0);

            /** Set int value with specified name */
            void setInt(const CString& name, int32 t, uint32 index = 0);

            /** Set float value with specified name */
            void setFloat(const CString& name, float t, uint32 index = 0);

            /** Set float vector value with specified name */
            void setVec2(const CString& name, const Vec2& t, uint32 index = 0);

            /** Set float vector value with specified name */
            void setVec3(const CString& name, const Vec3& t, uint32 index = 0);

            /** Set float vector value with specified name */
            void setVec4(const CString& name, const Vec4& t, uint32 index = 0);

            /** Set integer vector value with specified name */
            void setVec2i(const CString& name, const Vec2i& t, uint32 index = 0);

            /** Set integer vector value with specified name */
            void setVec3i(const CString& name, const Vec3i& t, uint32 index = 0);

            /** Set integer vector value with specified name */
            void setVec4i(const CString& name, const Vec4i& t, uint32 index = 0);

            /** Set matrix value with specified name */
            void setMat2(const CString& name, const Mat2& t, uint32 index = 0);

            /** Set matrix value with specified name */
            void setMat3(const CString& name, const Mat3& t, uint32 index = 0);

            /** Set matrix value with specified name  */
            void setMat4(const CString& name, const Mat4& t, uint32 index = 0);

            /** @return Optional instance name */
            const CString& getName() const { return mName; }

            /** @return Root material of this instance (newer null after valid construction) */
            class Material& getMaterial() const { return *mMaterial; }

        private:

            /** Optional instance name */
            CString mName;

            /** Root material of this instance (newer null after valid construction) */
            Material* mMaterial = nullptr;

            /** Layout of uniform data */
            const UniformBlockLayout* mLayout = nullptr;

            /** User exposed material data */
            UniformBuffer mMaterialData;
        };

    }
}

#endif //BERSERK_MATERIALINSTANCE_H

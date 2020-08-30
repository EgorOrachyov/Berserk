/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <MaterialInstance.h>

namespace Berserk {
    namespace Render {

        template <typename T,uint32 C>
        void setTVecN(const CString& name, const TVecN<T,C> &t, uint32 index, const UniformBlockLayout& layout, UniformBuffer& uniformBuffer) {
            auto param = layout.getMember(name);

            BERSERK_COND_ERROR_RET(param.isNotNull(), "Param (%s) is not present", name.data());
            BERSERK_COND_ERROR_RET(index < param->getElements(), "Out of array bounds array index for param (%s)", name.data());

            uint32 offset = param->getOffset();
            uint32 stride = param->getStride();

            uniformBuffer.setRaw(t.values, offset + stride * index, sizeof(T) * C);
        }

        template <typename T, uint32 C>
        void setTMatN(const CString& name, const TMatMxN<T,C,C> &t, uint32 index, const UniformBlockLayout& layout, UniformBuffer& uniformBuffer) {
            auto param = layout.getMember(name);

            BERSERK_COND_ERROR_RET(param.isNotNull(), "Param (%s) is not present", name.data());
            BERSERK_COND_ERROR_RET(index < param->getElements(), "Out of array bounds array index for param (%s)", name.data());

            TMatMxN<T,C,C> tt = param->getIsRowMajor()? t: t.transpose();
            uint32 offset = param->getOffset();
            uint32 stride = param->getStride();
            uint32 matrixStride = param->getMatrixStride();

            T* data = tt.values;
            uint32 writeOffset = offset + stride * index;

            for (uint32 i = 0; i < C; i++) {
                uniformBuffer.setRaw(data + C * i, writeOffset + matrixStride * i, sizeof(T) * C);
            }
        }

        MaterialInstance::MaterialInstance(CString name, Material &material) {
            mName = std::move(name);
            mMaterial = &material;
            mLayout = &mMaterial->getUniformUserLayout();
        }

        void MaterialInstance::setBool(const CString &name, bool t, uint32 index) {
            setTVecN(name, TVecN<bool,1>({t}), index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setInt(const CString &name, int32 t, uint32 index) {
            setTVecN(name, TVecN<int32,1>({t}), index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setFloat(const CString &name, float t, uint32 index) {
            setTVecN(name, TVecN<float ,1>({t}), index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec2(const CString &name, const MaterialInstance::Vec2 &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec3(const CString &name, const MaterialInstance::Vec3 &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec4(const CString &name, const MaterialInstance::Vec4 &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec2i(const CString &name, const MaterialInstance::Vec2i &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec3i(const CString &name, const MaterialInstance::Vec3i &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setVec4i(const CString &name, const MaterialInstance::Vec4i &t, uint32 index) {
            setTVecN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setMat2(const CString &name, const MaterialInstance::Mat2 &t, uint32 index) {
            setTMatN(name, t, index, *mLayout, mMaterialData);
        }

        void MaterialInstance::setMat3(const CString &name, const MaterialInstance::Mat3 &t, uint32 index) {
            setTMatN(name, t, index, *mLayout, mMaterialData);
        }
        
        void MaterialInstance::setMat4(const CString &name, const MaterialInstance::Mat4 &t, uint32 index) {
            setTMatN(name, t, index, *mLayout, mMaterialData);
        }

    }
}
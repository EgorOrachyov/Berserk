/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIFORMBUFFER_H
#define BERSERK_UNIFORMBUFFER_H

#include <Math/Vec4f.h>
#include <Math/Mat4x4f.h>
#include <RHI/RHIUniformBuffer.h>
#include <Containers/BinaryData.h>
#include <RenderResources/RenderResource.h>

namespace Berserk {
    namespace Rendering {

        /**
         * @brief Uniform buffer
         *
         * Provides access to RHI uniform buffer (structures), allows to modify typed
         * values from CPU side and update actual RHI resource on GPU.
         */
        class UniformBuffer : public RenderResource {
        public:

            using Vec2 = TVecN<float,2>;
            using Vec3 = TVecN<float,3>;
            using Vec4 = TVecN<float,4>;
            using Mat2 = TMatMxN<float,2,2>;
            using Mat3 = TMatMxN<float,3,3>;
            using Mat4 = TMatMxN<float,4,4>;

            /** Creates uniform buffer of specified size */
            UniformBuffer(uint32 size);
            ~UniformBuffer() override = default;

            bool isInitialized() const override;
            bool isInitializedRHI() const override;

            /** Set value with specified offset */
            void setFloat(float t, uint32 offset);

            /** Set value with specified offset */
            void setVec2(const Vec2& t, uint32 offset);

            /** Set value with specified offset */
            void setVec3(const Vec3& t, uint32 offset);

            /** Set value with specified offset */
            void setVec4(const Vec4& t, uint32 offset);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat2(const Mat2& t, uint32 offset, uint32 stride, bool transpose = true);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat3(const Mat3& t, uint32 offset, uint32 stride, bool transpose = true);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat4(const Mat4& t, uint32 offset, uint32 stride, bool transpose = true);

            /** @return True if CPU state is updated but not sync with RHI */
            bool isDirty() const { return mIsDirty; }

            /** @return If range in this buffer bounds*/
            bool isValidRange(uint32 offset, uint32 range) const { return (offset + range) <= getBufferSize(); };

            /** Sync GPU data with CPU if needed */
            void updateDataGPU();

            /** @return Size of buffer in bytes */
            uint32 getBufferSize() const { return mData.size(); }

            /** @return CPU buffer */
            const BinaryData& getData() const { return mData; }

            /** @return CPU buffer RHI resource */
            const TPtrShared<RHIUniformBuffer> &getRHI() const { return mUniformBufferRHI; }

        private:

            bool checkBounds(uint32 offset, uint32 valueSize);

            BinaryData mData;
            TPtrShared<RHIUniformBuffer> mUniformBufferRHI;
            mutable bool mIsDirty = false;

        };

    }
}



#endif //BERSERK_UNIFORMBUFFER_H

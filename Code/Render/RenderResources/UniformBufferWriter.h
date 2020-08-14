/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UNIFORMBUFFERWRITER_H
#define BERSERK_UNIFORMBUFFERWRITER_H

#include <Math/Vec4f.h>
#include <Math/Mat4x4f.h>
#include <RHI/RHIUniformBuffer.h>
#include <Containers/BinaryData.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Uniform buffer writer
         *
         * Provides access to CPU data as uniform data with type safe write.
         */
        class UniformBufferWriter {
        public:

            using Bool = bool ;
            using Vec2 = TVecN<float,2>;
            using Vec3 = TVecN<float,3>;
            using Vec4 = TVecN<float,4>;
            using Mat2 = TMatMxN<float,2,2>;
            using Mat3 = TMatMxN<float,3,3>;
            using Mat4 = TMatMxN<float,4,4>;

            /** Creates uniform buffer of specified size */
            UniformBufferWriter(uint32 size);
            UniformBufferWriter() = default;

            /** Resize uniform buffer and its CPU cache */
            void resize(uint32 size);

            /** Set value with specified offset */
            void setFloat(float t, uint32 offset);

            /** Set value with specified offset */
            void setVec2(const Vec2& t, uint32 offset);

            /** Set value with specified offset */
            void setVec3(const Vec3& t, uint32 offset);

            /** Set value with specified offset */
            void setVec4(const Vec4& t, uint32 offset);

            /** Set value with specified offset */
            void setBool(bool t, uint32 offset);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat2(const Mat2& t, uint32 offset, uint32 stride, bool transpose = true);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat3(const Mat3& t, uint32 offset, uint32 stride, bool transpose = true);

            /** Set value with specified offset, row stride and transpose, if needed */
            void setMat4(const Mat4& t, uint32 offset, uint32 stride, bool transpose = true);

            /** @return True if CPU state is updated */
            bool isDirty() const { return mIsDirty; }

            /** @return If range in this buffer bounds*/
            bool isValidRange(uint32 offset, uint32 range) const { return (offset + range) <= getBufferSize(); };

            /** Sync GPU data with CPU */
            void updateDataGPU(const TPtrShared<RHIUniformBuffer> &buffer, uint32 offset = 0);

            /** Mark as clean */
            void markClean() { mIsDirty = false; }

            /** @return Size of buffer in bytes */
            uint32 getBufferSize() const { return mData.size(); }

            /** @return CPU buffer */
            const BinaryData& getData() const { return mData; }

        private:

            bool checkBounds(uint32 offset, uint32 valueSize);

            BinaryData mData;
            mutable bool mIsDirty = false;

        };

    }
}




#endif //BERSERK_UNIFORMBUFFERWRITER_H

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_DYNAMICUNIFORMBUFFER_H
#define BERSERK_DYNAMICUNIFORMBUFFER_H

#include <Containers/BinaryData.h>
#include <RHI/RHIUniformBuffer.h>
#include <RHI/RHIDevice.h>

namespace Berserk {
    namespace Render {

        /** Dynamic uniform buffer for frequently updated and resized uniform data */
        class DynamicUniformBuffer {
        public:

            /** How scale buffer when resize*/
            static const uint32 EXPAND_FACTOR = 2;

            void write(uint32 offset, const uint8* data, uint32 size) { mBinaryData.write(offset, data, size); }
            void append(uint32 bytes) { mBinaryData.resize(mBinaryData.size() + bytes); }

            /** Appends specified data to CPU cache */
            void append(const uint8* data, uint32 size) { mBinaryData.add(data, size); }

            /** Add trivial element via simple memory copy */
            template <typename T>
            void append(const T& element) { mBinaryData.add((const uint8*) &element, sizeof(T)); }

            /** Clear CPU cache buffer */
            void clear() { mBinaryData.clear(); }

            /**
             * Update actual data on GPU (possibly allocate new RHI buffer)
             * @return True if reallocated new buffer
             */
            bool updateGPU() {
                auto size = mBinaryData.size();
                auto data = mBinaryData.data();

                auto reallocate = expand(size);
                mUniformBuffer->update(size, 0, data);

                return reallocate;
            }

            bool updateGPU(uint32 offset, uint32 range) {
                BERSERK_COND_ERROR_RET_VALUE(false, offset + range <= getSize(), "An attempt to update out of buffer range");

                auto size = mBinaryData.size();
                auto data = mBinaryData.data();

                auto reallocate = expand(size);
                mUniformBuffer->update(range, offset, data + offset);

                return reallocate;
            }

            /**
             * Pre-allocates GPU buffer of the specified size
             * @return True if reallocated new RHI buffer
             */
            bool preallocateGPU(uint32 size) { return expand(size); }

            /** @return Buffer size */
            uint32 getSize() const { return mBinaryData.size(); }

            /** @return CPU cached data */
            const BinaryData& getData() const { return mBinaryData; }

            /** @return CPU cached data */
            BinaryData& getData() { return mBinaryData; }

            /** @return GPU RHI buffer (might be null if no data were sync with GPU) */
            const TPtrShared<RHIUniformBuffer> &getRHI() const { return mUniformBuffer; }

            /** @return GPU RHI buffer (might be null if no data were sync with GPU) */
            TPtrShared<RHIUniformBuffer> &getRHI() { return mUniformBuffer; }

        private:

            bool expand(uint32 size) {
                if (mUniformBuffer.isNull() || mUniformBuffer->getBufferSize() < size) {
                    auto bufferSize = getSizeByFactor(size, EXPAND_FACTOR);
                    auto bufferUsage = EBufferUsage::Dynamic;

                    mUniformBuffer = RHIDevice::getSingleton().createUniformBuffer(bufferSize, bufferUsage, nullptr);
                    return true;
                }

                return false;
            }

            static uint32 getSizeByFactor(uint32 size, uint32 factor) {
                uint32 s = 2;
                while (s < size) s *= factor;
                return s;
            }

            BinaryData mBinaryData;
            TPtrShared<RHIUniformBuffer> mUniformBuffer;

        };

    }
}

#endif //BERSERK_DYNAMICUNIFORMBUFFER_H

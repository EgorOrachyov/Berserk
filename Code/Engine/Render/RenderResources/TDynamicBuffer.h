/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TDYNAMICBUFFER_H
#define BERSERK_TDYNAMICBUFFER_H

#include <RHI/RHIVertexBuffer.h>
#include <RHI/RHIDevice.h>
#include <Containers/BinaryData.h>

namespace Berserk {
    namespace Render {

        /** Dynamic buffer for frequently updated and resized data */
        template <typename Buffer>
        class TDynamicBuffer {
        public:

            /** How scale buffer when resize*/
            static const uint32 EXPAND_FACTOR = 2;

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

                auto reallocate = expand<Buffer>(size);
                mBuffer->update(size, 0, data);

                return reallocate;
            }

            /**
             * Pre-allocates GPU buffer of the specified size
             * @return True if reallocated new RHI buffer
             */
            bool preallocateGPU(uint32 size) { return expand(size); }

            /** @return CPU cached data */
            const BinaryData& getData() const { return mBinaryData; }

            /** @return CPU cached data */
            BinaryData& getData() { return mBinaryData; }

            /** @return GPU RHI buffer (might be null if no data were sync with GPU) */
            const TPtrShared<Buffer> &getRHI() const { return mBuffer; }

            /** @return GPU RHI buffer (might be null if no data were sync with GPU) */
            TPtrShared<Buffer> &getRHI() { return mBuffer; }

        private:

            template <typename B>
            bool expand(uint32 size);

            template <>
            bool expand<RHIVertexBuffer>(uint32 size) {
                if (mBuffer.isNull() || mBuffer->getBufferSize() < size) {
                    auto bufferSize = getSizeByFactor(size, EXPAND_FACTOR);
                    auto bufferUsage = EBufferUsage::Dynamic;

                    mBuffer = RHIDevice::getSingleton().createVertexBuffer(bufferSize, bufferUsage, nullptr);
                    return true;
                }

                return false;
            }

            template <>
            bool expand<RHIIndexBuffer>(uint32 size) {
                if (mBuffer.isNull() || mBuffer->getBufferSize() < size) {
                    auto bufferSize = getSizeByFactor(size, EXPAND_FACTOR);
                    auto bufferUsage = EBufferUsage::Dynamic;

                    mBuffer = RHIDevice::getSingleton().createIndexBuffer(bufferSize, bufferUsage, nullptr);
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
            TPtrShared<Buffer> mBuffer;

        };

    }
}

#endif //BERSERK_TDYNAMICBUFFER_H

/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BINARYDATA_H
#define BERSERK_BINARYDATA_H

#include <Containers/TArray.h>

namespace Berserk {

    /**
     * @brief Binary data
     *
     * Binary data container for raw byte data stored as uint8 values.
     * Supports fast IO operations, but unsafe for byte-order sensitive data.
     */
    class BinaryData final {
    public:
        BinaryData(IAlloc& alloc = IAlloc::getSingleton()) : mData(alloc) {};
        ~BinaryData() { mData.clearNoDestructorCall(); }

        void resize(uint32 size) { mData.resize(size); }
        void ensureCapacity(uint32 desired) { mData.ensureCapacity(desired); }
        void ensureToAdd(uint32 count) { mData.ensureToAdd(count); }

        void add(const uint8* buffer, uint32 bufferSize);
        void write(uint32 offset, const uint8* buffer, uint32 bufferSize);
        void clear() { mData.clearNoDestructorCall(); }

        uint32 size() const { return mData.size(); }

        uint8* data() { return mData.data(); }
        const uint8* data() const { return mData.data(); }

        TArray<uint8> &internal() { return mData; }
        const TArray<uint8> &internal() const { return mData; }

        /** Raw fast serialization for binary array data */
        friend Archive& operator<<(Archive& archive, const BinaryData &data) {
            auto elements = data.size();
            archive << elements;
            archive.vwrite(data.data(), elements * sizeof(uint8));
            return archive;
        }

        /** Raw fast deserialization for binary array data */
        friend Archive& operator>>(Archive& archive, BinaryData &data) {
            uint32 elements = 0;
            archive >> elements;
            data.resize(elements);
            archive.vread(data.data(), elements * sizeof(uint8));
            return archive;
        }

    private:
        TArray<uint8> mData;
    };

}

#endif //BERSERK_BINARYDATA_H
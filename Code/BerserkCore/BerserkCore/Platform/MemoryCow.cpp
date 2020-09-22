/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/MemoryCow.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {

    MemoryCow::MetaData::MetaData(uint64 dataSize)
        : mReferences(0),
          mBufferSize(dataSize) {

    }

    uint64 MemoryCow::MetaData::addRefs() {
        return mReferences.fetch_add(1);
    }

    uint64 MemoryCow::MetaData::release() {
        return mReferences.fetch_sub(1);
    }

    uint64 MemoryCow::MetaData::getReferences() {
        return mReferences.load();
    }

    uint64 MemoryCow::MetaData::getBufferSize() {
        return mBufferSize;
    }

    void* MemoryCow::MetaData::getData() const {
        return (void*)((this) + 1);
    }

    MemoryCow::MemoryCow(const void *data, uint64 size) {
        mMetaData = createMeta(size, data);
        mMetaData->addRefs();
    }

    MemoryCow::MemoryCow(const MemoryCow &other) {
        if (other.mMetaData) {
            mMetaData = other.mMetaData;
            mMetaData->addRefs();
        }
    }

    MemoryCow::MemoryCow(MemoryCow &&other) noexcept {
        if (other.mMetaData) {
            mMetaData = other.mMetaData;
            other.mMetaData = nullptr;
        }
    }

    MemoryCow::~MemoryCow() {
        if (mMetaData) {
            tryRelease(mMetaData);
            mMetaData = nullptr;
        }
    }

    MemoryCow& MemoryCow::operator=(const MemoryCow &other) {
        if (&other == this)
            return *this;

        this->~MemoryCow();
        new (this) MemoryCow(other);
        return *this;
    }

    MemoryCow& MemoryCow::operator=(MemoryCow &&other) noexcept {
        if (&other == this)
            return *this;

        this->~MemoryCow();
        new (this) MemoryCow(std::move(other));
        return *this;
    }

    uint64 MemoryCow::getBufferSize() const {
        return mMetaData? mMetaData->getBufferSize(): 0;
    }

    uint64 MemoryCow::getReferencesCount() const {
        return mMetaData? mMetaData->getReferences(): 0;
    }

    const void* MemoryCow::getDataReadonly() const {
        return mMetaData? mMetaData->getData(): nullptr;
    }

    void* MemoryCow::getData() {
        if (mMetaData) {
            if (mMetaData->getReferences() > 1) {
                auto prevMeta = mMetaData;
                mMetaData = cloneMeta(prevMeta);
                mMetaData->addRefs();
                tryRelease(prevMeta);
            }

            return mMetaData->getData();
        }

        return nullptr;
    }

    MemoryCow::MetaData* MemoryCow::createMeta(uint64 dataSize, const void *data) {
        uint64 toAllocate = sizeof(MetaData) + dataSize;
        auto memory = (uint8*) Application::getSingleton().getGlobalAllocator().allocate(toAllocate);

        auto meta = new (memory) MetaData(dataSize);
        if (data) Memory::copy(memory + sizeof(MetaData), data, dataSize);

        return meta;
    }

    MemoryCow::MetaData* MemoryCow::cloneMeta(MemoryCow::MetaData *metaToClone) {
        uint64 dataSize = metaToClone->getBufferSize();
        const void* data = metaToClone->getData();

        return createMeta(dataSize, data);
    }

    void MemoryCow::tryRelease(MemoryCow::MetaData *meta) {
        if (meta) {
            auto refsBefore = meta->release();

            // We are the last one who releases
            if (refsBefore == 1) {
                // Remember to call destructor
                meta->~MetaData();
                Application::getSingleton().getGlobalAllocator().free(meta);
            }
        }
    }

}
//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_OBJECTID_H
#define BERSERK_OBJECTID_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * An index and magic number universal object index. Allows to store object
     * in the array buffer at the same index and remove properly.
     * Increments magic number each time, when resource at index is released.
     */
    class ObjectID
    {
    public:

        /** Empty object id */
        ObjectID()
        {

        }

        explicit ObjectID(uint64 index)
            : mIndex(index), mMagicNumber(0)
        {

        }

        ObjectID(uint64 index, uint64 magicNumber)
            : mIndex(index), mMagicNumber(magicNumber)
        {

        }

        const bool operator==(const ObjectID& id) const
        {
            return mIndex == id.mIndex && mMagicNumber == id.mMagicNumber;
        }

        const bool operator!=(const ObjectID& id) const
        {
            return mIndex != id.mIndex || mMagicNumber != id.mMagicNumber;
        }

        /** @return Index of this id */
        const uint64 getIndex() const
        {
            return mIndex;
        }

        /** @return Magic number of this id */
        const uint64 getMagicNumber() const
        {
            return mMagicNumber;
        }

        /** @return New object id at the same index */
        ObjectID createNew()
        {
            return ObjectID(mIndex, mMagicNumber + 1);
        }

    private:

        uint64 mIndex = 0;
        uint64 mMagicNumber = 0;

    };

} // namespace Berserk

#endif //BERSERK_OBJECTID_H

//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TMAP_H
#define BERSERK_TMAP_H

#include "TIterator.h"

namespace Berserk
{

    /**
     * Template pair type for map storage
     *
     * @tparam K Template type of key
     * @tparam V Template type of value
     */
    template <typename K, typename V>
    class TPair
    {
    public:

        TPair(const K& key, const V& value)
        {
            memcpy(&mKey, &key, sizeof(K));
            memcpy(&mValue, &value, sizeof(V));
        }

        void update(const V& value)
        {
            memcpy(&mValue, &value, sizeof(V));
        }

        K& getKey()
        {
            return mKey;
        }

        V& value()
        {
            return mValue;
        }

    private:

        K mKey;
        V mValue;

    };

    /**
     * Map interface for associative collections to store
     * key value pairs. Requires (==) operator for keys.
     *
     * @tparam K Template type of key
     * @tparam V Template type of value
     */
    template <typename K, typename V>
    class TMap : public TIterator<TPair<K,V>>
    {
    public:

        /**
         * Checks whether the key is presented in the map
         * @param key Key to check
         * @return True if key is stored in the map, otherwise false
         */
        virtual bool contains(const K& key) = 0;

        /**
         * Puts new (key,value) pair in the map, replaces
         * previously added pair, rewrites values
         * @param key Key to place
         * @param value Value to associate with key
         */
        virtual void put(const K& key, const V& value) = 0;

        /**
         * Allows to access value via key
         * @param key Key to access associated value
         * @return Pointer to the value or null, if there is
         *         nothing stored with key
         */
        virtual V* get(const K& key) const = 0;

        /**
         * @return Current number of elements in the container
         */
        virtual uint32 getSize() const = 0;

        /**
         * @return Range of hashing for storing elements
         */
        virtual uint32 getRange() const = 0;

        /**
         * @return Size of node in the map (size of pair (key,value))
         */
        virtual uint32 getNodeSize() const = 0;

        /**
         * @return Memory usage in bytes by this container
         */
        virtual uint32 getMemoryUsage() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_TMAP_H
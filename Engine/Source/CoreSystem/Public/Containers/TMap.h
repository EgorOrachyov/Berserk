//
// Created by Egor Orachyov on 02.06.2019.
//

#ifndef BERSERK_TMAP_H
#define BERSERK_TMAP_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Containers/TIterator.h>

namespace Berserk
{

    /**
     * Template pair type for map storage
     *
     * @tparam K Template type of key
     * @tparam V Template type of value
     */
    template <typename K, typename V>
    class CORE_EXPORT TPair
    {
    public:

        GENERATE_NEW_DELETE(TPair);

        /** Empty pair */
        TPair()
        {}

        /** Copy in raw buffers */
        TPair(const K& key, const V& value)
        {
            memcpy(mKey, &key, sizeof(K));
            memcpy(mValue, &value, sizeof(V));
        }

        ~TPair()
        {
            key()->~K();
            value()->~V();
        }

        /**
         * Copy new content to the value
         * and destroy previous
         */
        void update(const V& value)
        {
            value()->~V();
            memcpy(mValue, &value, sizeof(V));
        }

        /** @return Pointer to the key */
        K* key() { return (K*)mKey; }

        /** @return Pointer to the value */
        V* value() { return (V*)mValue; }

    private:

        uint8 mKey[sizeof(K)];
        uint8 mValue[sizeof(V)];

    };

    /**
     * Map interface for associative collections to store
     * key value pairs. Requires compare predicate operator for keys.
     *
     * @tparam K Template type of key
     * @tparam V Template type of value
     */
    template <typename K, typename V>
    class CORE_EXPORT TMap : public TIterator<TPair<K,V>>
    {
    public:

        /**
         * Declare virtual destructor for containers hierarchy
         */
        virtual ~TMap() = default;

        /**
         * Checks whether the key is presented in the map
         * @param key Key to check
         * @return True if key is stored in the map, otherwise false
         */
        virtual bool contains(const K& key) const = 0;

        /**
         * Puts new (key,value) pair in the map, replaces
         * previously added pair, rewrites only value
         *
         * @param key Key to place
         * @param value Value to associate with key
         */
        virtual void put(const K& key, const V& value) = 0;

        /**
         * Allows to create complex object, which does not support movement
         * semantic in the memory or has complex structure
         * (for example: containers, strings, resources...)
         *
         * Puts new (key,created value) pair in the map, replaces
         * previously added pair, rewrites only value
         *
         * @warning T type of object must support new/delete semantic of the engine
         *
         * @tparam TArgs Type of arguments, used to create new instance of object T
         * @param args Actual arguments, which will be used to create new instance
         */
        template <typename ... TArgs>
        void emplace(const K& key, const TArgs& ... args)
        {
            V* value = get(key);
            if (value == nullptr)
            {
                TPair<K,V>* memory = emplace_uninitialized(key);
                memcpy(memory->key(), &key, sizeof(K));
                new (memory->value()) V(args ...);
            }
            else
            {
                value->~V();
                new (value) V(args ...);
            }
        }

        /**
         * Allows to create complex object, which does not support movement
         * semantic in the memory or has complex structure
         * (for example: containers, strings, resources...)
         *
         * Puts new (key,created value) pair in the map, replaces
         * previously added pair, rewrites values
         *
         * @warning T type of object must support new/delete semantic of the engine
         *
         * @tparam TArgs Type of arguments, used to create new instance of object T
         * @param args Actual arguments, which will be used to create new instance
         */
        template <typename Arg, typename ... TArgs>
        void emplace(const Arg& arg, const TArgs& ... args)
        {
            uint8 keyMem[sizeof(K)];
            K* key = new (keyMem) K(arg);

            V* value = get(*key);
            if (value == nullptr)
            {
                TPair<K,V>* memory = emplace_uninitialized(*key);
                memcpy(memory->key(), key, sizeof(K));
                new (memory->value()) V(args ...);
            }
            else
            {
                key->~K();
                value->~V();
                new (value) V(args ...);
            }
        }

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
         * @return Memory usage in bytes by this container
         */
        virtual uint32 getMemoryUsage() const = 0;

    protected:

        /**
         * Preallocate raw uninitialized pair in the map
         * @note If the container is full, will expand buffer
         * @return Pointer to pre-allocated pair
         */
        virtual TPair<K,V>* emplace_uninitialized(const K &key) = 0;

    };

} // namespace Berserk

#endif //BERSERK_TMAP_H
/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TMAP_H
#define BERSERK_TMAP_H

#include <Typedefs.h>
#include <TIterable.h>

namespace Berserk {

    template <typename K, typename V>
    class TMap {
    public:
        virtual ~TMap() = default;
        /** Adds element in the map. Replace old value if present with new one */
        virtual void add(const K& key, const V& value) = 0;
        /**
         * Adds element in the map, if it does not contains element with the same key
         * @return True if elements successfully added, otherwise false
         */
        virtual bool addIfNotPresent(const K& key, const V& value) = 0;
        /** @return True f map contatins element with the provided key */
        virtual bool contains(const K& key) const = 0;
        /**
         * Removes element with key.
         * @return True if removed, otherwise false
         */
        virtual bool remove(const K& key) = 0;
        /** Removes all teh elements in the map */
        virtual void clear() = 0;
        /** @return Pointer to found value otherwise null */
        virtual V* getPtr(const K& key) = 0;
        /** @return Pointer to found value otherwise null */
        virtual const V* getPtr(const K& key) const = 0;
        /** @return Reference to the value with key */
        virtual V& operator[](const K& key) = 0;
        /** @return Reference to the value with key */
        virtual const V& operator[](const K& key) const = 0;
        /** @return Number of the pairs (key,value) in the map */
        virtual uint32 size() const = 0;
    };

}

#endif //BERSERK_TMAP_H
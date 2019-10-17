/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_TMAP_H
#define BERSERK_TMAP_H

#include <TPair.h>
#include <TVariant.h>

namespace Berserk
{

    /**
     * Map interface for associative collection to store
     * key value pairs. Requires compare predicate operator for keys.
     *
     * @tparam K Template type of key
     * @tparam V Template type of value
     */
    template <typename K, typename V>
    class TMap
    {
    public:

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
         * @return Placed value reference
         */
        virtual V& put(const K& key, const V& value) = 0;

        /**
         * Add uninitialized value in the map
         * @param key Key to place
         * @return Pointer to value memory
         */
        virtual V* putUninitialized(const K &key) = 0;

        /**
         * Allows to access value via key
         * @param key Key to access associated value
         * @return Variant, contains pointer to the value
         */
        virtual TVariant<V*> get(const K& key) const = 0;

        /**
         * Removes element from map by key
         * @param key Key to access associated value
         * @return True, if element was found and removed
         */
        virtual bool remove(const K& key) = 0;

        /** @return Current number of elements in the container */
        virtual uint32 getSize() const = 0;

        /** @return Memory usage in bytes by this container */
        virtual uint32 getMemoryUsage() const = 0;

        /**
         * Construct value from provided arguments and places
         * in the map with associated key value.
         * @tparam TArgs Types of arguments to construct value
         * @param key Key value
         * @param args Arguments to construct value
         * @return Placed value reference
         */
        template <typename ... TArgs>
        V& emplace(const K& key, TArgs&& ... args) {
            TVariant<V*> v = get(key);
            if (v.isNull()) {
                V* mem = putUninitialized(key);
                new (mem) V(std::forward<TArgs>(args) ...);
            } else {
                new (v.get()) V(std::forward<TArgs>(args) ...);
            }
        }

    };

} // namespace Berserk

#endif //BERSERK_TMAP_H
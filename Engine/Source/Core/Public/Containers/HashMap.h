//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_HASHMAP_H
#define BERSERK_HASHMAP_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Assert.h"
#include "Public/Memory/Allocator.h"

namespace Berserk
{

    template <typename K, typename V>
    class HashMap
    {
    public:

        /** Does not support small ranges for hashing efficiency */
        static const MIN_HASH_RANGE = 64;

    public:

        explicit HashMap(uint32 range = MIN_HASH_RANGE);

        ~HashMap();

        void remove(const K& key);

        void empty();

        void operator += (const K& key, const V& value);

        V*   operator [] (const K& key) const;

    };


} // namespace Berserk

#endif //BERSERK_HASHMAP_H
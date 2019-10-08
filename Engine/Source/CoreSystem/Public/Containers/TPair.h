//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_TPAIR_H
#define BERSERK_TPAIR_H

#include <HAL/Types.h>
#include <TComparator.h>

namespace Berserk {

    /**
     * Generic pair class
     * (uses emplacement for copy of the key and value objects)
     *
     * @tparam K Key type
     * @tparam V Value type
     */
    template <typename K, typename V>
    class TPair {
    public:

        TPair(const K& key, const V& value) {
            new (mBufferKey) K(key);
            new (mBufferValue) V(value);
        }

        ~TPair() {
            getKey().~K();
            getValue().~V();
        }

        K& getKey() const {
            return *((K*)mBufferKey);
        }

        V& getValue() const {
            return *((V*)mBufferValue);
        }

    private:

        int8 mBufferKey[sizeof(K)];
        int8 mBufferValue[sizeof(V)];

    };

}




#endif //BERSERK_TPAIR_H

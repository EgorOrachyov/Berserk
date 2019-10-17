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

#ifndef BERSERKTESTS_TPAIR_H
#define BERSERKTESTS_TPAIR_H

#include <HAL/Platform.h>

namespace Berserk {

    /**
     * Generic pair class to store key/value pairs
     * @tparam K Key object type
     * @tparam V Value object type
     */
    template <typename K, typename V>
    class TPair {
    public:

        TPair(const K& key, const V& value) {
            new (mBufferKey) K(key);
            new (mBufferValue) V(value);
        }

        TPair(const TPair<K,V> &other) {
            new (mBufferKey) K(other.getKey());
            new (mBufferValue) V(other.getValue());
        }

        TPair(TPair<K,V>&& other) noexcept {
            new (mBufferKey) K(std::move(other.getKey()));
            new (mBufferValue) V(std::move(other.getValue()));
        }

        ~TPair() {
            getKey().~K();
            getValue().~V();
        }

        TPair<K,V> &operator=(const TPair<K,V> &other) {
            ~TPair();
            new (mBufferKey) K(other.getKey());
            new (mBufferValue) V(other.getValue());
            return *this;
        }

        bool operator==(const TPair<K,V> &other) const {
            return (getKey() == other.getKey()) && (getValue() == other.getValue());
        }

        bool operator!=(const TPair<K,V> &other) const {
            return (getKey() != other.getKey()) || (getValue() != other.getValue());
        }

        const K& getKey() const {
            return *((K*)mBufferKey);
        }

        const V& getValue() const {
            return *((V*)mBufferValue);
        }

        K& getKey() {
            return *((K*)mBufferKey);
        }

        V& getValue() {
            return *((V*)mBufferValue);
        }

    private:
        char mBufferKey[sizeof(K)] = {};
        char mBufferValue[sizeof(V)] = {};
    };

}

#endif //BERSERKTESTS_TPAIR_H
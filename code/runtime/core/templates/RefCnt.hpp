/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
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

#ifndef BERSERK_REFCNT_HPP
#define BERSERK_REFCNT_HPP

#include <core/Config.hpp>

#include <atomic>
#include <cassert>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class RefCnt
 * @brief Reference counted base object
 *
 * Inherit from this class to have shared-ref logic for your class objects.
 * Use RefPtr to wrap and automate RefCnt objects references counting.
 *
 * @see Ref
 */
class BRK_API RefCnt {
public:
    virtual ~RefCnt() {
#ifdef BERSERK_DEBUG
        assert(mRefs.load() == 0);
        mRefs.store(0);
#endif
    }

    bool IsUnique() const {
        return GetRefs() == 1;
    }

    std::int32_t GetRefs() const {
        return mRefs.load(std::memory_order_relaxed);
    }

    std::int32_t AddRef() const {
        assert(GetRefs() >= 0);
        return mRefs.fetch_add(1);
    }

    std::int32_t RelRef() const {
        assert(GetRefs() > 0);
        auto refs = mRefs.fetch_sub(1);

        if (refs == 1) {
            // Was last reference
            // Destroy object and release memory
            delete this;
        }

        return refs;
    }

private:
    // This type of object after creation always has no references
    mutable std::atomic_int32_t mRefs{0};
};

/**
 * Unsafe shared object reference
 *
 * @tparam T Type of object
 * @param object Object to reference
 * @return Object reference
 */
template<typename T>
static inline T *AddRef(T *object) {
    assert(object);
    object->AddRef();
    return object;
}

/**
 * Safe shared object reference
 *
 * @tparam T Type of object
 * @param object Object to reference
 * @return Object reference
 */
template<typename T>
static inline T *SafeAddRef(T *object) {
    if (object)
        object->AddRef();
    return object;
}

/**
 * Shared object release reference
 *
 * @tparam T Type of object
 * @param object Object to be unreferenced
 */
template<typename T>
static inline void Unref(T *object) {
    if (object)
        object->RelRef();
}

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_REFCNT_HPP

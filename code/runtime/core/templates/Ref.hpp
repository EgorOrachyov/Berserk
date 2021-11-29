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

#ifndef BERSERK_REF_HPP
#define BERSERK_REF_HPP

#include <core/templates/RefCnt.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Ref
 * @brief Automates reference counting and behaves as shared smart pointer.
 * @tparam T Type referenced object
 */
template<typename T>
class BRK_API Ref {
public:
    Ref() = default;

    explicit Ref(T *object) {
        if (object)
            mObject = AddRef(object);
    }

    Ref(const Ref &other) {
        if (other.mObject)
            mObject = AddRef(other.mObject);
    }

    Ref(Ref &&other) noexcept {
        mObject = other.mObject;
        other.mObject = nullptr;
    }

    ~Ref() {
        Unref(mObject);
        mObject = nullptr;
    }

    Ref<T> &operator=(const Ref &other) {
        if (this != &other)
            this->Reset(SafeRef(other.Get()));
        return *this;
    }

    Ref<T> &operator=(Ref &&other) noexcept {
        if (this != &other)
            this->Reset(other.Release());
        return *this;
    }

    bool operator==(const Ref &other) const {
        return mObject == other.mObject;
    }

    [[nodiscard]] bool IsNull() const {
        return mObject == nullptr;
    }

    [[nodiscard]] bool IsNotNull() const {
        return mObject;
    }

    T *operator->() const {
        assert(mObject);
        return mObject;
    }

    T &operator*() const {
        assert(mObject);
        return *mObject;
    }

    explicit operator bool() const {
        return mObject != nullptr;
    }

    void Reset(T *ptr = nullptr) {
        T *old = mObject;
        mObject = ptr;
        Unref(old);
    }

    T *Release() {
        T *ptr = mObject;
        mObject = nullptr;
        return ptr;
    }

    T *Get() const {
        return mObject;
    }

    template<typename G>
    [[nodiscard]] bool Is() const {
        return !mObject || dynamic_cast<G *>(mObject);
    }

    template<class G>
    Ref<G> As() const {
        return Ref<G>(mObject);
    }

    template<class G>
    Ref<G> Cast() const {
        return Ref<G>(dynamic_cast<G *>(mObject));
    }

private:
    T *mObject = nullptr;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_REF_HPP
